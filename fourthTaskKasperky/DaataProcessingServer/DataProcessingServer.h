#pragma once
#include <string>
#include <winsock2.h>

class DataProcessingServer {
public:
    DataProcessingServer(int listenPort, const std::string& resultServerIP, int resultServerPort);
    ~DataProcessingServer();
    void run();

private:
    bool connectToResultServer();
    void processClient(SOCKET clientSocket);
    std::string processData(const std::string& data);

    int listenPort;
    std::string resultServerIP;
    int resultServerPort;
    SOCKET serverSocket;
    SOCKET resultServerSocket;
};
