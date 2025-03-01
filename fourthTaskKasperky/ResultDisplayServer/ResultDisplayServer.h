#pragma once
#include <iostream>
#include <string>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

class ResultDisplayServer {
public:
    ResultDisplayServer(int port);
    ~ResultDisplayServer();
    void run();

private:
    int port;
    SOCKET serverSocket;
    void displayResult(SOCKET clientSocket);
};