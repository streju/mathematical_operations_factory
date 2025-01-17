#pragma once

#include <map>
#include <memory>
#include <mutex>

#include "IWarehouse.hpp"
#include "IWarehouseEntryPoint.hpp"
#include "Operation.hpp"
#include "Tools/ThreadSafeQueue.hpp"

class WarehouseEntryPoint : public IWarehouseEntryPoint
{
public:
    WarehouseEntryPoint(const std::shared_ptr<IWarehouse>& warehouse);

    void placeOperResult(const OperationResultPtr& operationResult) override;
    void movePendings() override;
    ProductPtr notifyAboutNewTransporter(unsigned transporterNr) override;
    WarehouseRaport getState() override;
    void startLoadTransport(const std::string& prefix) override;
private:
    struct State
    {
        explicit State(unsigned nrOfNeededWorkers) : nrOfNeededWorkers(nrOfNeededWorkers)
        {}
        bool isReady() {return nrOfNeededWorkers == nrOfAssignedWorkers;}
        void assignWorker() {nrOfAssignedWorkers++;}
    private:
        unsigned nrOfNeededWorkers;
        std::atomic<unsigned> nrOfAssignedWorkers{0};
    };

    void startPlacing(const OperationResultPtr& operationResult);

    std::shared_ptr<IWarehouse> warehouse_;
    tools::ThreadSafeQueue<OperationResultPtr> pendings_;
    std::map<unsigned, State> operationToState_;
    std::mutex mutex_;
};
