#include "Manager.hpp"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>
#include "src/Tools/Logger.hpp"

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
    Logger() << prefix_ << "::" << __func__ << std::endl;
    while (true)
    {
        int operationTypeNr = rand() % 4;
        Logger() << operationTypeNr << std::endl;
        operationNr_++;
        auto operation = translateToOperation(operationTypeNr);
        if (!operation)
        {
            Logger() << prefix_ << ": Cannot create Operation" << std::endl;
            continue;
        }
        auto operationPtr = std::make_shared<Operation>(*operation);
        queue_->push(operationPtr);
        std::this_thread::sleep_for(std::chrono::seconds(rand() % maxTimeOfSleep_ + minTimeOfSleep_));
    }
}

std::optional<Operation> Manager::translateToOperation(int operationTypeNr)
{
    Logger() <<  prefix_ << " creats operation nr ";
    switch (operationTypeNr)
    {
        case 0:
            Logger() << operationNr_ << ": addition" <<std::endl;
            return Operation(Operation::Type::addition, operationNr_);
        case 1:
            Logger() << operationNr_ << ": substraction" <<std::endl;
            return Operation(Operation::Type::substraction, operationNr_);
        case 2:
            Logger() << operationNr_ << ": multiplication" <<std::endl;
            return Operation(Operation::Type::multiplication, operationNr_);
        case 3:
            Logger() << operationNr_ << ": division" <<std::endl;
            return Operation(Operation::Type::division, operationNr_);
        default:
            Logger() << "unsupported operation!" <<std::endl;
            return std::nullopt;
    }
}
