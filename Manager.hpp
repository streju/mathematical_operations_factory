#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <optional>
#include <string>

#include "SafeQueue.hpp"
#include "Operation.hpp"

class Manager
{
public:
    Manager(int minTimeOfSleep, int maxTimeOfSleep, std::shared_ptr<ThreadSafeQueue> queue);
    void start();
private:
    std::optional<Operation> translateToOperation(int operationTypeNr);

    int maxTimeOfSleep_;
    int minTimeOfSleep_;
    std::shared_ptr<ThreadSafeQueue> queue_;
    int operationNr_;
    std::string prefix_;
};

#endif // MANAGER_HPP
