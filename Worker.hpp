#ifndef WORKER_HPP
#define WORKER_HPP

#include <string>
#include "SafeQueue.hpp"
#include "Operation.hpp"
#include "OperationMachine.hpp"

class Worker
{
public:
    Worker(int nr, std::shared_ptr<ThreadSafeQueue> queue, std::shared_ptr<OperationMachine> machine);
    void tryAct();
private:
    void transportToMachine(std::shared_ptr<Operation> operation);

    std::shared_ptr<ThreadSafeQueue> queue_;
    std::string prefix_;
};

#endif // WORKER_HPP
