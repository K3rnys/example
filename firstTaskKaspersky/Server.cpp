#include "Server.h"
#include "Session.h"
#include <iostream>

Server::Server(boost::asio::io_context& io_context, unsigned short port): 
    _ioContext(io_context),
    _acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
    _startAccept();
    }

// Ожидание входящих соединений, создание сокетов
void Server::_startAccept() {
    _acceptor.async_accept([this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
        if (!ec) {
            std::make_shared<Session>(std::move(socket), *this)->start();
        }
        _startAccept();
        });
}

// Остановка сервера
void Server::stop() {
    _ioContext.stop();
}