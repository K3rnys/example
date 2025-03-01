#include "ResultDisplayServer.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <Port>\n";
        return 1;
    }

    int port = std::stoi(argv[1]);

    ResultDisplayServer server(port);
    server.run();

    return 0;
}
