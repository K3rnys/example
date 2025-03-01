#pragma once
#include <boost/asio.hpp>
#include <memory>

class Server {
public:
    Server(boost::asio::io_context& io_context, unsigned short port);
    void stop();  

private:
    void _startAccept();

    boost::asio::io_context& _ioContext;
    boost::asio::ip::tcp::acceptor _acceptor;
};