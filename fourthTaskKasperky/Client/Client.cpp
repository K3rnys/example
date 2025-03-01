#include "Client.h"

Client::Client(const std::string& serverIP, int serverPort)
    : serverIP(serverIP), serverPort(serverPort), clientSocket(INVALID_SOCKET) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
}

Client::~Client() {
    closesocket(clientSocket);
    WSACleanup();
}

bool Client::connectToServer() {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    InetPtonA(AF_INET, serverIP.c_str(), &serverAddr.sin_addr);  // Use InetPtonA instead of InetPton

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        closesocket(clientSocket);
        return false;
    }
    return true;
}

void Client::run() {
    std::string userInput;
    while (true) {
        std::cout << "Enter data: ";
        std::getline(std::cin, userInput);

        if (userInput.empty()) {
            continue;
        }

        send(clientSocket, userInput.c_str(), userInput.size(), 0);
        char buffer[1024] = { 0 };
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesReceived > 0) {
            std::cout << "Server response: " << std::string(buffer, 0, bytesReceived) << std::endl;
        }
    }
}
