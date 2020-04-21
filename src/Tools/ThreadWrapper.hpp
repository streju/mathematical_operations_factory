#pragma once

#include <thread>
#include "Logger.hpp"
class ThreadWrapper
{
public:
    ThreadWrapper(){}
    ThreadWrapper(const ThreadWrapper& other) = delete;
    ThreadWrapper(std::thread&& thread, bool joinable)
        : thread_(std::move(thread))
        , joinable_(joinable)
    {
    }
    ~ThreadWrapper()
    {
        Logger("ThreadWrapper") << "DTOR" << std::endl;
        if (thread_.joinable() && joinable_)
        {
            thread_.join();
        }
        else
        {
            thread_.detach();
        }
    }
    std::thread& get()
    {
        return thread_;
    }
private:
    std::thread thread_;
    bool joinable_;
};
