#include "Logger.hpp"

Logger::~Logger()
{
    std::scoped_lock<std::mutex> lock(logMutex_);
    std::cout << this->str();
}

std::mutex Logger::logMutex_{};
