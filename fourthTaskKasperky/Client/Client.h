
#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>  // Include ws2tcpip.h for InetPton

#pragma comment(lib, "ws2_32.lib")

class Client {
public:
    Client(const std::string& serverIP, int serverPort);
    ~Client();
    bool connectToServer();
    void run();

private:
    std::string serverIP;
    int serverPort;
    SOCKET clientSocket;
};