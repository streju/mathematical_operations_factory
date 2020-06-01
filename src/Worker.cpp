#include "Worker.hpp"

#include <iostream>

#include "Tools/Randoms.hpp"
#include "Tools/Timer.hpp"

Worker::Worker(const MachinesServicePtr& machinesService,
    const WarehouseEntryPointPtr& warehouse,
    const tools::ProgramStopControllerPtr& stopController)
    : machinesService_(machinesService)
    , warehouse_(warehouse)
    , stopController_(stopController)
{
}

void Worker::startProcessingOperation(const unsigned threadNr, const OperationPtr& operation)
{
    std::string prefix{"Worker nr" + std::to_string(threadNr) + ": "};
    Logger(prefix) << "Starts working on operation: " << *operation << std::endl;
    transportToMachine(prefix, operation);

    auto futureResult = machinesService_->registerOperation(operation);
    if (stopController_->wasStopRequested())
    {
        Logger(prefix) << "Application stop was requested, end work." << std::endl;
        return;
    }
    if (!futureResult.valid())
    {
        Logger(prefix) << "ERROR during calculation of operation: " << *operation << std::endl;
        return;
    }
    auto result = futureResult.get();
    Logger(prefix) << "End of calculation in the machine, operation: " << *operation << std::endl;
    putOperationResultToMagazine(prefix, operation, result);
}

void Worker::loadTransport(unsigned threadNr)
{
    std::string prefix{"Worker nr" + std::to_string(threadNr) + ": "};
    Logger(prefix) << "Got request to load transporter" << std::endl;
    warehouse_->startLoadTransport(prefix);
}

void Worker::movePendingsOperationsToWarehouse(unsigned threadNr)
{
    std::string prefix{"Worker nr" + std::to_string(threadNr) + ": "};
    Logger(prefix) << "Got request to move pendings operations results to warehouse" << std::endl;
    warehouse_->movePendings();
}

void Worker::transportToMachine(const std::string& prefix, const OperationPtr& operation) const
{
    tools::SecondsTimer timer;
    std::this_thread::sleep_for(std::chrono::seconds(tools::random(2, 6)));
    Logger(prefix) << "Transport to machine, operation: " << *operation << " end. Time: "
        << timer.stopAndGetTime() << "s." << std::endl;
}

void Worker::putOperationResultToMagazine(const std::string& prefix, const OperationPtr& operation,
    const OperationResultPtr& operationResult)
{
    tools::SecondsTimer timer;
    std::this_thread::sleep_for(std::chrono::seconds(tools::random(2, 6)));
    Logger(prefix) << "Transport to magazine, operation: " << *operation << " end. Time: "
        << timer.stopAndGetTime() << "s." << std::endl;

    warehouse_->placeOperResult(operationResult);
}
