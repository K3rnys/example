#include "Session.h"
#include "Server.h"
#include "handler.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>

Session::Session(boost::asio::ip::tcp::socket socket, Server& server): _socket(std::move(socket)), _server(server) {}

// ��������� �� ������������� ����������
void Session::start() {
    std::cout << "Session started" << std::endl;
    _doRead();
}

void Session::_doRead() {
    // ����� ��������� �� ������� ������
    auto self(shared_from_this());

    // ������ ������ �� ������
    boost::asio::async_read_until(
        _socket, _buffer, '\n', [this, self](boost::system::error_code exception, std::size_t length) {
            if (!exception) {
                //������ ������ �� �������
                std::istream is(&_buffer);
                std::string request;
                std::getline(is, request);

                // �������� '\r', ���� ��� ����
                if (!request.empty() && request.back() == '\r') {
                    request.pop_back();
                }

                // ���� �� ������
                std::istringstream iss(request);
                std::string command, hash, filePath;
                iss >> command >> hash >> filePath;

                // ��������� ������
                std::string response = _processRequest(command, hash, filePath);
                _doWrite(std::make_shared<std::string>(response));
            }
            else {
                // ������ ������
                std::cerr << "Error on read: " << exception.message() << std::endl;
            }
        }
    );
}

std::string Session::_processRequest(const std::string& command, const std::string& hash, const std::string& filePath) {
    if (command == "quit") {
        // �������� �������
        _server.stop();
        return "Server shutting down\n";
    }
    if (command == "get" && hash == "hash" && !filePath.empty()) {
        // ������ �� �����
        std::ifstream file(filePath);
        if (file) { 
            try {
                // ����������� �����
                auto handler = std::make_unique<Handler>(filePath);
                return handler->getMD5() + "\n";
            }
            catch (const std::exception& exception) {
                // �������� ������
                return "Error: " + std::string(exception.what()) + "\n";
            }
        }
        else {
            // ����� �������
            return "Failed to open file: " + filePath + "\n";
        }
    }
    else {
        // ��� ����� �������
        return "Invalid command\n";
    }
}

void Session::_doWrite(std::shared_ptr<std::string> response) {
    // ����� ��������� �� ������� ������ �2
    auto self(shared_from_this());

    // ������ ������ � �����
    boost::asio::async_write(
        _socket, boost::asio::buffer(*response), [this, self, response](boost::system::error_code exception, std::size_t length) {
            if (!exception) {
                // ������� ������
                _buffer.consume(_buffer.size());
                _doRead();
            }
            else {
                // ������ ������
                std::cerr << "Error on write: " << exception.message() << std::endl;
                _socket.close();
            }
        }
    );
}