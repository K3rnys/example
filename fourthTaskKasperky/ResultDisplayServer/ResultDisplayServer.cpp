#include "ResultDisplayServer.h"
#include <thread>

ResultDisplayServer::ResultDisplayServer(int port)
    : port(port), serverSocket(INVALID_SOCKET) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
}

ResultDisplayServer::~ResultDisplayServer() {
    closesocket(serverSocket);
    WSACleanup();
}

void ResultDisplayServer::run() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket.\n";
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Error binding socket.\n";
        return;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Error listening on socket.\n";
        return;
    }

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket != INVALID_SOCKET) {
            std::thread(&ResultDisplayServer::displayResult, this, clientSocket).detach();
        }
    }
}

void ResultDisplayServer::displayResult(SOCKET clientSocket) {
    char buffer[1024] = { 0 };
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        std::string data(buffer, 0, bytesReceived);
        std::cout << "Received processed data: " << data << std::endl;
    }
    closesocket(clientSocket);
}
