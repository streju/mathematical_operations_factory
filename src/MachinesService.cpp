#include "MachinesService.hpp"

#include "Tools/Logger.hpp"

MachinesService::MachinesService(const std::shared_ptr<tools::IProgramStopControllerHelper>& stopController, unsigned nrOfMachines)
    : machinesPool_(stopController, nrOfMachines), stopController_(stopController), prefix_("MachinesService: ")
{
    Logger(prefix_) << nrOfMachines << " machines to use." << std::endl;
}

std::shared_future<OperationResultPtr>
MachinesService::registerOperation(OperationPtr operation)
{
    operation->assignWorker();
    if (!operation->isEnoughWorkers())
    {
        Logger(prefix_) << "Worker will wait to another" << std::endl;
        std::unique_lock<std::mutex> lk(mu_);
        cv_.wait(lk, [operation, this](){
            return operation->isEnoughWorkers() || stopController_->wasStopRequested();
        });
    }
    else if (operation->getOperationType() == Operation::Type::multiplication ||
        operation->getOperationType() == Operation::Type::division)
    {
        Logger(prefix_) << "Notify other worker" << std::endl;
        cv_.notify_one();
    }

    std::scoped_lock lk(mu_);
    if (operationNrToFuture_.find(operation->getOperationNr()) == operationNrToFuture_.end())
    {
        operationNrToFuture_[operation->getOperationNr()] = FutureResult(
            machinesPool_.pushToQueue(std::bind(&Machine::performCalculation, &machine_, operation, std::placeholders::_1)));
        return operationNrToFuture_.at(operation->getOperationNr());
    }

    const auto shFuture = operationNrToFuture_.at(operation->getOperationNr());
    operationNrToFuture_.erase(operation->getOperationNr());
    return shFuture;
}
