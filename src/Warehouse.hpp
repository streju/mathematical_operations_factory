#pragma once

#include <map>
#include <memory>
#include <mutex>
#include <condition_variable>

#include "Tools/IProgramStopControllerHelper.hpp"
#include "Tools/ThreadSafeQueue.hpp"
#include "IWarehouse.hpp"

class Warehouse : public IWarehouse
{
public:
    Warehouse(unsigned maxSize, const tools::ProgramStopControllerPtr& stopController);

    void put(const OperationResultPtr& operationResult) override;
    ProductPtr handleTransporterArrival(unsigned transporterNr) override;
    void startLoadTransport(const std::string& prefix) override;
    bool isFull() override;
    bool isAnyTransporterWaiting() override;
private:
    struct TransportHelper
    {
        std::atomic<bool> isReady{false};
        ProductPtr product;
    };

    bool isTransportLoaded(unsigned transporterNr);
    bool isTransporterWaiting(unsigned transporterNr);

    tools::ThreadSafeQueue<OperationResultPtr> depot_;
    std::map<unsigned, TransportHelper> transportersToLoad_;
    unsigned maxSize_;
    tools::ProgramStopControllerPtr stopController_;
    std::mutex mutexForTransport_;
    std::condition_variable cv_;
};
