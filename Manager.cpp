#include "Manager.hpp"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>

Manager::Manager(int minTimeOfSleep, int maxTimeOfSleep, std::shared_ptr<ThreadSafeQueue> queue)
    : minTimeOfSleep_(minTimeOfSleep)
    , maxTimeOfSleep_(maxTimeOfSleep)
    , queue_(queue)
    , operationNr_(0)
    , prefix_("Manager")
{
}

void Manager::start()
{
    std::cout << prefix_ << "::" << __func__ << std::endl;
    while (true)
    {
        int operationTypeNr = rand() % 4;
        std::cout << operationTypeNr << std::endl;
        operationNr_++;
        auto operation = translateToOperation(operationTypeNr);
        if (!operation)
        {
            std::cout << prefix_ << ": Cannot create Operation" << std::endl;
            continue;
        }
        auto operationPtr = std::make_shared<Operation>(*operation);
        queue_->push(operationPtr);
        std::this_thread::sleep_for(std::chrono::seconds(rand() % maxTimeOfSleep_ + minTimeOfSleep_));
    }
}

std::optional<Operation> Manager::translateToOperation(int operationTypeNr)
{
    std::cout <<  prefix_ << " creats operation nr ";
    switch (operationTypeNr)
    {
        case 0:
            std::cout << operationNr_ << ": addition" <<std::endl;
            return Operation(Operation::Type::addition, operationNr_);
        case 1:
            std::cout << operationNr_ << ": substraction" <<std::endl;
            return Operation(Operation::Type::substraction, operationNr_);
        case 2:
            std::cout << operationNr_ << ": multiplication" <<std::endl;
            return Operation(Operation::Type::multiplication, operationNr_);
        case 3:
            std::cout << operationNr_ << ": division" <<std::endl;
            return Operation(Operation::Type::division, operationNr_);
        default:
            std::cout << "unsupported operation!" <<std::endl;
            return std::nullopt;
    }
}
