#include "Manager.hpp"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>
#include <optional>
#include "Tools/Logger.hpp"
#include "Tools/Randoms.hpp"

Manager::Manager(const std::shared_ptr<tools::IProgramStopControllerHelper>& stopController,
    std::unique_ptr<tools::WorkersPool>&& workersPool, const std::shared_ptr<IMachinesService>& machinesService,
    const std::shared_ptr<IWarehouseEntryPoint>& warehouse, const std::shared_ptr<IWorker>& worker, unsigned minTimeOfSleep, unsigned maxTimeOfSleep)
    : stopController_(stopController)
    , workersPool_(std::move(workersPool))
    , machinesService_(machinesService)
    , warehouse_(warehouse)
    , worker_(worker)
    , maxTimeOfSleep_(maxTimeOfSleep)
    , minTimeOfSleep_(minTimeOfSleep)
    , operationNr_(0)
    , prefix_("Manager")
{
}

void Manager::start()
{
    Logger() << prefix_ << "::" << __func__ << std::endl;

    while (!stopController_->wasStopRequested())
    {
        const auto warehouseRaport = warehouse_->getState();
        if (warehouseRaport.isAnyTransporterWaiting)
        {
            workersPool_->pushToQueue(funcToRunTransport_, WorkerHelper::WorkType::Transport);
        }
        if (!warehouseRaport.isDepotFull)
        {
            if (warehouseRaport.isAnyOperationPending)
            {
                workersPool_->pushToQueue(funcToMovePendingsToWarehouse_, WorkerHelper::WorkType::Transport);
            }
            startOperation();
        }
        if (warehouseRaport.isDepotFull && !warehouseRaport.isAnyTransporterWaiting && !warehouseRaport.isAnyOperationPending)
        {
            Logger(prefix_) << " Warehouse is full and lack of waiting transport, no action will be performed" << std::endl;
        }
        auto time = tools::random(minTimeOfSleep_, maxTimeOfSleep_);
        Logger(prefix_) << " Will wait " << time << " seconds with creations of next operation" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(time));
    }
}

void Manager::startOperation()
{
    operationNr_++;
    auto operationPtr = createOperation();
    if (!operationPtr)
    {
        Logger() << prefix_ << ": Cannot create Operation" << std::endl;
        return;
    }
    auto funcToRunOperation = [this, operationPtr](auto threadNr){worker_->startProcessingOperation(threadNr, operationPtr);};
    workersPool_->pushToQueue(funcToRunOperation, WorkerHelper::WorkType::Calculation, operationPtr->getOperationType());
}

OperationPtr Manager::createOperation() const
{
    Logger() << prefix_ << " creats operation nr " << operationNr_;
    const auto& operType = tools::select_randomly_from_container(possibleOperTypes_.begin(), possibleOperTypes_.end());
    switch (*operType)
    {
        case Operation::Type::addition:
            Logger() << ": addition" <<std::endl;
            return std::make_shared<Operation>(Operation::Type::addition, operationNr_);
        case Operation::Type::substraction:
            Logger() << ": substraction" <<std::endl;
            return std::make_shared<Operation>(Operation::Type::substraction, operationNr_);
        case Operation::Type::multiplication:
            Logger() << ": multiplication" <<std::endl;
            return std::make_shared<Operation>(Operation::Type::multiplication, operationNr_);
        case Operation::Type::division:
            Logger() << ": division" <<std::endl;
            return std::make_shared<Operation>(Operation::Type::division, operationNr_);
        default:
            Logger() << "unsupported operation!" <<std::endl;
            return nullptr;
    }
}
