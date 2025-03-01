#include <iostream>
#include <string>
#include "DataProcessingServer.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <listen_port> <result_server_ip> <result_server_port>" << std::endl;
        return 1;
    }

    int listenPort = std::stoi(argv[1]);
    std::string resultServerIP = argv[2];
    int resultServerPort = std::stoi(argv[3]);

    DataProcessingServer server(listenPort, resultServerIP, resultServerPort);
    server.run();

    return 0;
}
