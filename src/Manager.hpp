#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <array>
#include <optional>
#include <string>

#include "Operation.hpp"
#include "MachinesService.hpp"
#include "IWarehouseEntryPoint.hpp"
#include "Tools/WorkersPool.hpp"
#include "Tools/IProgramStopControllerHelper.hpp"

#include "Worker.hpp"

class Manager
{
public:
    Manager(const std::shared_ptr<tools::IProgramStopControllerHelper>& stopController,
        std::unique_ptr<tools::WorkersPool>&& workersPool, const std::shared_ptr<IMachinesService>& machinesService,
        const std::shared_ptr<IWarehouseEntryPoint>& warehouse, const std::shared_ptr<IWorker>& worker, unsigned minTimeOfSleep, unsigned maxTimeOfSleep);
    void start();
    ~Manager(){Logger(prefix_) << "DTOR" << std::endl;}

private:
    void startOperation();
    OperationPtr createOperation() const;

    const std::shared_ptr<tools::IProgramStopControllerHelper> stopController_;
    const std::unique_ptr<tools::WorkersPool> workersPool_;
    const std::shared_ptr<IMachinesService> machinesService_;
    const std::shared_ptr<IWarehouseEntryPoint> warehouse_;
    const std::shared_ptr<IWorker> worker_;
    const unsigned maxTimeOfSleep_;
    const unsigned minTimeOfSleep_;
    int operationNr_;
    const std::string prefix_;
    const std::function<void(unsigned)> funcToRunTransport_{[this](auto threadNr){worker_->loadTransport(threadNr);}};
    const std::function<void(unsigned)> funcToMovePendingsToWarehouse_{[this](auto threadNr){worker_->movePendingsOperationsToWarehouse(threadNr);}};
    const std::array<Operation::Type, 4> possibleOperTypes_{
        Operation::Type::addition, Operation::Type::substraction, Operation::Type::multiplication, Operation::Type::division};
};

#endif // MANAGER_HPP
