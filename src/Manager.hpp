#pragma once

#include <array>
#include <string>

#include "Operation.hpp"
#include "IMachinesService.hpp"
#include "IWarehouseEntryPoint.hpp"
#include "IWorker.hpp"
#include "Tools/IProgramStopControllerHelper.hpp"
#include "WorkersPool.hpp"


class Manager
{
public:
    Manager(const tools::ProgramStopControllerPtr& stopController,
        std::unique_ptr<WorkersPool>&& workersPool, const MachinesServicePtr& machinesService,
        const WarehouseEntryPointPtr& warehouse, const WorkerPtr& worker, unsigned minTimeOfSleep, unsigned maxTimeOfSleep);
    void start();

private:
    void startOperation();
    OperationPtr createOperation() const;

    const tools::ProgramStopControllerPtr stopController_;
    const std::unique_ptr<WorkersPool> workersPool_;
    const MachinesServicePtr machinesService_;
    const WarehouseEntryPointPtr warehouse_;
    const WorkerPtr worker_;
    const unsigned maxTimeOfSleep_;
    const unsigned minTimeOfSleep_;
    int operationNr_;
    const std::string prefix_;
    const std::function<void(unsigned)> funcToRunTransport_{[this](auto threadNr){worker_->loadTransport(threadNr);}};
    const std::function<void(unsigned)> funcToMovePendingsToWarehouse_{[this](auto threadNr){worker_->movePendingsOperationsToWarehouse(threadNr);}};
    const std::array<Operation::Type, 4> possibleOperTypes_{
        Operation::Type::addition, Operation::Type::substraction, Operation::Type::multiplication, Operation::Type::division};
};
