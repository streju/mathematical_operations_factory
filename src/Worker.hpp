#ifndef WORKER_HPP
#define WORKER_HPP

#include <memory>
#include <string>
#include <utility>
#include <optional>

#include "IWorker.hpp"
#include "Operation.hpp"
#include "OperationResult.hpp"
#include "IMachinesService.hpp"
#include "IWarehouseEntryPoint.hpp"
#include "Tools/ThreadsPool.hpp"
#include "Tools/IProgramStopControllerHelper.hpp"

class Worker : public IWorker
{
public:
    Worker(const MachinesServicePtr& machinesService,
        const std::shared_ptr<IWarehouseEntryPoint>& warehouse,
        const std::shared_ptr<tools::IProgramStopControllerHelper>& stopController);
    void startProcessingOperation(const unsigned threadNr, const OperationPtr& operation) override;
    void loadTransport(unsigned threadNr) override;
    void movePendingsOperationsToWarehouse(unsigned threadNr) override;
    ~Worker(){Logger("Worker") << "DTOR" << std::endl;}
private:
    void transportToMachine(const std::string& prefix, const OperationPtr& operation) const;
    void putOperationResultToMagazine(const std::string& prefix, const OperationPtr& operation,
        const OperationResultPtr& operationResult);

    MachinesServicePtr machinesService_;
    std::shared_ptr<IWarehouseEntryPoint> warehouse_;
    std::shared_ptr<tools::IProgramStopControllerHelper> stopController_;
};

#endif // WORKER_HPP
