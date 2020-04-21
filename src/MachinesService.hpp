#ifndef MACHINESSERVICE_HPP
#define MACHINESSERVICE_HPP

#include <map>
#include <string>
#include <utility>
#include <thread>
#include <vector>
#include <functional>
#include <mutex>
#include <condition_variable>

#include "IMachinesService.hpp"
#include "Operation.hpp"
#include "OperationResult.hpp"
#include "Tools/ThreadsPool.hpp"
#include "Machines.hpp"


class MachinesService : public IMachinesService
{
public:
    MachinesService(const std::shared_ptr<tools::IProgramStopControllerHelper>& stopController, unsigned nrOfMachines);
    std::shared_future<OperationResultPtr> registerOperation(OperationPtr operation) override;
    ~MachinesService(){Logger(prefix_) << "DTOR" << std::endl;}
private:
    using FutureResult = std::shared_future<OperationResultPtr>;

    tools::ThreadPool machinesPool_;
    std::shared_ptr<tools::IProgramStopControllerHelper> stopController_;
    Machine machine_;
    std::string prefix_;
    std::map<unsigned, FutureResult> operationNrToFuture_;
    std::mutex mu_;
    std::condition_variable cv_;
};

#endif // MACHINESSERVICE_HPP
