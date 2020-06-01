#include "WarehouseEntryPoint.hpp"

#include <algorithm>
#include <utility>
#include <thread>

#include "Tools/Randoms.hpp"

WarehouseEntryPoint::WarehouseEntryPoint(const std::shared_ptr<IWarehouse>& warehouse) : warehouse_(warehouse)
{}

void WarehouseEntryPoint::placeOperResult(const OperationResultPtr& operationResult)
{
    const auto& operationNr = operationResult->operationNr_;
    std::unique_lock<std::mutex> lk(mutex_);

    startPlacing(operationResult);

    if (operationToState_.find(operationNr) == operationToState_.end())
    {
        Logger() << "Error durign putting OperationResult to depot." << std::endl;
        std::abort();
        return;
    }

    operationToState_.at(operationNr).assignWorker();
    if (!operationToState_.at(operationNr).isReady())
    {
        Logger() << "End working on operationNr: " << operationNr << ". Wait for second worker." << std::endl;
        return;
    }
    operationToState_.erase(operationNr);
    lk.unlock();

    if (!warehouse_->isFull())
    {
        warehouse_->put(operationResult);
        return;
    }
    Logger() << "Warehouse is full, move operation nr " << operationNr << " to pendings." << std::endl;
    pendings_.push(operationResult);
}

void WarehouseEntryPoint::movePendings()
{
    if (warehouse_->isFull())
    {
        Logger() << "Warehouse is full, cannot move pendings operations." << std::endl;
        return;
    }

    OperationResultPtr result;
    if (!pendings_.try_pop(result))
    {
        Logger() << "There are no more pendings operations to move to warehouse." << std::endl;
        return;
    }

    warehouse_->put(result);
    movePendings();
}

WarehouseRaport WarehouseEntryPoint::getState()
{
    return {warehouse_->isFull(), warehouse_->isAnyTransporterWaiting(), !pendings_.empty()};
}

ProductPtr WarehouseEntryPoint::notifyAboutNewTransporter(unsigned transporterNr)
{
    return warehouse_->handleTransporterArrival(transporterNr);
}

void WarehouseEntryPoint::startLoadTransport(const std::string& prefix)
{
    warehouse_->startLoadTransport(prefix);
}

void WarehouseEntryPoint::startPlacing(const OperationResultPtr& operationResult)
{
    unsigned nrOfNeededWorkers;
    if (operationToState_.find(operationResult->operationNr_) == operationToState_.end())
    {
        if (operationResult->type_ == Operation::Type::multiplication || operationResult->type_ == Operation::Type::division)
        {
            nrOfNeededWorkers = 2;
        }
        else
        {
            nrOfNeededWorkers = 1;
        }
        operationToState_.emplace(std::piecewise_construct,
            std::forward_as_tuple(operationResult->operationNr_),
            std::forward_as_tuple(nrOfNeededWorkers));
    }
}
