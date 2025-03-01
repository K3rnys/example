#include "Session.h"
#include "Server.h"
#include "handler.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>

Session::Session(boost::asio::ip::tcp::socket socket, Server& server): _socket(std::move(socket)), _server(server) {}

// Сообщение об инициализации соединения
void Session::start() {
    std::cout << "Session started" << std::endl;
    _doRead();
}

void Session::_doRead() {
    // Умный указатель на текущий объект
    auto self(shared_from_this());

    // Чтение данных из сокета
    boost::asio::async_read_until(
        _socket, _buffer, '\n', [this, self](boost::system::error_code exception, std::size_t length) {
            if (!exception) {
                //Чтение данных из буффера
                std::istream is(&_buffer);
                std::string request;
                std::getline(is, request);

                // Удаление '\r', если оно есть
                if (!request.empty() && request.back() == '\r') {
                    request.pop_back();
                }

                // Ввод из потока
                std::istringstream iss(request);
                std::string command, hash, filePath;
                iss >> command >> hash >> filePath;

                // Обработка команд
                std::string response = _processRequest(command, hash, filePath);
                _doWrite(std::make_shared<std::string>(response));
            }
            else {
                // Ошибка чтения
                std::cerr << "Error on read: " << exception.message() << std::endl;
            }
        }
    );
}

std::string Session::_processRequest(const std::string& command, const std::string& hash, const std::string& filePath) {
    if (command == "quit") {
        // Закрытие сервера
        _server.stop();
        return "Server shutting down\n";
    }
    if (command == "get" && hash == "hash" && !filePath.empty()) {
        // Чтение из файла
        std::ifstream file(filePath);
        if (file) { 
            try {
                // Хеширование файла
                auto handler = std::make_unique<Handler>(filePath);
                return handler->getMD5() + "\n";
            }
            catch (const std::exception& exception) {
                // Перехват ошибок
                return "Error: " + std::string(exception.what()) + "\n";
            }
        }
        else {
            // Часть отладки
            return "Failed to open file: " + filePath + "\n";
        }
    }
    else {
        // Еще часть отладки
        return "Invalid command\n";
    }
}

void Session::_doWrite(std::shared_ptr<std::string> response) {
    // Умный указатель на текущий объект х2
    auto self(shared_from_this());

    // Запись данных в сокет
    boost::asio::async_write(
        _socket, boost::asio::buffer(*response), [this, self, response](boost::system::error_code exception, std::size_t length) {
            if (!exception) {
                // Очистка буфера
                _buffer.consume(_buffer.size());
                _doRead();
            }
            else {
                // Ошибка записи
                std::cerr << "Error on write: " << exception.message() << std::endl;
                _socket.close();
            }
        }
    );
}