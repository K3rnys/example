#include "Server.h"
#include <boost/asio.hpp>
#include <iostream>

int main() {
    try {
        boost::asio::io_context io_context;
        unsigned short port;
        std::cout << "Enter the port number: ";
        std::cin >> port;

        // Инициализация сервера на указанном порте
        Server server(io_context, port);
        io_context.run();
    }
    catch (const std::exception& exception) {
        // Сообщение об ошибке
        std::cerr << "Exception: " << exception.what() << std::endl;
    }
    return 0;
}   