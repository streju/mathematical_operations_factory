#pragma once

#include <iostream>
#include <sstream>
#include <mutex>

class Logger : public std::ostringstream
{
public:
    Logger() = default;
    explicit Logger(const std::string& prefix) : prefix_(prefix) {}
    ~Logger();
private:
    static std::mutex logMutex_;
    const std::string prefix_;
};

// TODO change to queue of messages
// TODO add different level of prints
