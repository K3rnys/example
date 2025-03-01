#include "Client.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <ServerIP> <ServerPort>\n";
        return 1;
    }

    std::string serverIP = argv[1];
    int serverPort = std::stoi(argv[2]);

    Client client(serverIP, serverPort);

    if (!client.connectToServer()) {
        std::cerr << "Failed to connect to server.\n";
        return 1;
    }

    client.run();

    return 0;
}