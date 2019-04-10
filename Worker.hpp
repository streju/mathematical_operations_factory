#ifndef WORKER_HPP
#define WORKER_HPP

#include <string>
#include "SafeQueue.hpp"
#include "Operation.hpp"

class Worker
{
public:
    Worker(int nr, std::shared_ptr<ThreadSafeQueue> queue);
    void tryAct();
private:
    std::shared_ptr<ThreadSafeQueue> queue_;
    std::string prefix_;
};

#endif // WORKER_HPP
