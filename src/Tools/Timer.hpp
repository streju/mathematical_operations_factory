#pragma once

#include <chrono>

namespace tools
{

class SecondsTimer
{
    std::chrono::time_point<std::chrono::steady_clock> start_;
public:
    SecondsTimer() : start_(std::chrono::steady_clock::now())
    {}

    double stopAndGetTime() const
    {
        const auto stop = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::seconds>(stop - start_).count();
    }
};

} // namespace tools
