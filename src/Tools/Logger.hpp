#pragma once

#include <iostream>
#include <sstream>
#include <mutex>

//class Logger
//{
//public:
//    Logger()
//    {
//        mu_.lock();
//    }
//    ~Logger()
//    {
//        mu_.unlock();
//    }
//private:
//    std::mutex mu_;
//};

// other way to log
//std::stringstream msg;
//msg << "jakies " << "mesgi" << "\n";
//std::cout << msg.str();

class Logger : public std::ostringstream
{
public:
    Logger() = default;
    ~Logger();
private:
    static std::mutex logMutex_;
};

// queue of message??


// Asynchronous output
//struct acout
//{
//        std::unique_lock<std::mutex> lk;
//        acout()
//            :
//              lk(std::unique_lock<std::mutex>(mtx_cout))
//        {

//        }

//        template<typename T>
//        acout& operator<<(const T& _t)
//        {
//            std::cout << _t;
//            return *this;
//        }

//        acout& operator<<(std::ostream& (*fp)(std::ostream&))
//        {
//            std::cout << fp;
//            return *this;
//        }
//};
