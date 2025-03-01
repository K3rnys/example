#include "handler.h"
#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <stdexcept>

Handler::Handler(const std::string& filePath) : _filePath(filePath) {}

std::string Handler::getMD5() const {
    // Открытие файла в бинарном режиме
    std::ifstream file(_filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Unable to open file: " + _filePath);
    }

    // Запись данных из буфера в строку
    std::ostringstream oss;
    oss << file.rdbuf();
    std::string fileContents = oss.str();

    // Инициализация
    boost::uuids::detail::md5 hash;
    boost::uuids::detail::md5::digest_type digest;

    // Хеширование
    hash.process_bytes(fileContents.data(), fileContents.size());
    hash.get_digest(digest);

    // Преобразование хеша в строку в 16-ом формате
    std::ostringstream result;
    for (const auto& byte : digest) {
        result << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
    }

    return result.str();
}