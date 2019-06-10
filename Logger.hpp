#pragma once

#include <mutex>

class Logger
{
public:
	Logger();
	~Logger();
private:
    std::mutex mu;
};

