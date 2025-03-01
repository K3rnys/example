#pragma once
#include <string>

class Handler {
public:
    explicit Handler(const std::string& filePath);
    std::string getMD5() const;

private:
    std::string _filePath;
};

