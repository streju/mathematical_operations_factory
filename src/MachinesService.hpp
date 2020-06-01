#pragma once

#include <condition_variable>
#include <map>
#include <mutex>
#include <string>
#include <utility>

#include "IMachinesService.hpp"
#include "Machines.hpp"
#include "Operation.hpp"
#include "OperationResult.hpp"
#include "Tools/ThreadsPool.hpp"


class MachinesService : public IMachinesService
{
public:
    MachinesService(const tools::ProgramStopControllerPtr& stopController, unsigned nrOfMachines);
    std::shared_future<OperationResultPtr> registerOperation(OperationPtr operation) override;
private:
    using FutureResult = std::shared_future<OperationResultPtr>;

    tools::ThreadPool machinesPool_;
    tools::ProgramStopControllerPtr stopController_;
    Machine machine_;
    std::string prefix_;
    std::map<unsigned, FutureResult> operationNrToFuture_;
    std::mutex mu_;
    std::condition_variable cv_;
};
