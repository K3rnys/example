#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <string>

class Server;  

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(boost::asio::ip::tcp::socket socket, Server& server);
    void start();

private:
    void _doRead();
    void _doWrite(std::shared_ptr<std::string> response);
    std::string _processRequest(const std::string& command, const std::string& hash, const std::string& filePath);

    boost::asio::ip::tcp::socket _socket;
    boost::asio::streambuf _buffer;
    Server& _server;
};