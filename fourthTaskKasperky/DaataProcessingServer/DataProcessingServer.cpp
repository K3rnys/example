#include "DataProcessingServer.h"
#include <iostream>
#include <thread>
#include <sstream>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

DataProcessingServer::DataProcessingServer(int listenPort, const std::string& resultServerIP, int resultServerPort)
    : listenPort(listenPort), resultServerIP(resultServerIP), resultServerPort(resultServerPort), serverSocket(INVALID_SOCKET), resultServerSocket(INVALID_SOCKET) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
}

DataProcessingServer::~DataProcessingServer() {
    closesocket(serverSocket);
    closesocket(resultServerSocket);
    WSACleanup();
}

void DataProcessingServer::run() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket.\n";
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(listenPort);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Error binding socket.\n";
        return;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Error listening on socket.\n";
        return;
    }

    if (!connectToResultServer()) {
        std::cerr << "Failed to connect to result server.\n";
        return;
    }

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket != INVALID_SOCKET) {
            std::thread(&DataProcessingServer::processClient, this, clientSocket).detach();
        }
    }
}

bool DataProcessingServer::connectToResultServer() {
    resultServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (resultServerSocket == INVALID_SOCKET) {
        return false;
    }

    sockaddr_in resultServerAddr;
    resultServerAddr.sin_family = AF_INET;
    resultServerAddr.sin_port = htons(resultServerPort);
    InetPtonA(AF_INET, resultServerIP.c_str(), &resultServerAddr.sin_addr);

    if (connect(resultServerSocket, (sockaddr*)&resultServerAddr, sizeof(resultServerAddr)) == SOCKET_ERROR) {
        closesocket(resultServerSocket);
        return false;
    }
    return true;
}

void DataProcessingServer::processClient(SOCKET clientSocket) {
    char buffer[1024] = { 0 };
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        std::string data(buffer, 0, bytesReceived);
        std::string processedData = processData(data);

        send(clientSocket, "Data received and processed.", 29, 0);

        send(resultServerSocket, processedData.c_str(), processedData.size(), 0);
    }
    closesocket(clientSocket);
}

std::string DataProcessingServer::processData(const std::string& data) {
    std::istringstream iss(data);
    std::vector<std::string> words;
    std::string word;
    std::unordered_set<std::string> uniqueWords;

    while (iss >> word) {
        uniqueWords.insert(word);
    }

    for (const auto& w : uniqueWords) {
        words.push_back(w);
    }

    std::sort(words.begin(), words.end());

    std::ostringstream oss;
    for (const auto& w : words) {
        oss << w << " ";
    }

    return oss.str();
}
