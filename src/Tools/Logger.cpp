#include "Logger.hpp"

Logger::~Logger()
{
    std::scoped_lock<std::mutex> lock(logMutex_);
    if (prefix_.empty()) std::cout << this->str();
    else std::cout << prefix_ << this->str();
}

std::mutex Logger::logMutex_{};
