#pragma once

#include <map>
#include <memory>
#include <mutex>
#include <vector>
#include <condition_variable>

#include "Operation.hpp"
#include "OperationResult.hpp"
#include "Product.hpp"
#include "Tools/IProgramStopControllerHelper.hpp"
#include "Tools/ThreadSafeQueue.hpp"
#include "IWarehouse.hpp"

class Warehouse : public IWarehouse
{
public:
    Warehouse(unsigned maxSize, const std::shared_ptr<tools::IProgramStopControllerHelper>& stopController);

    void put(const OperationResultPtr& operationResult) override;
    ProductPtr handleTransporterArrival(unsigned transporterNr) override;
    void startLoadTransport(const std::string& prefix) override;
    bool isFull() override;
    bool isAnyTransporterWaiting() override;
    ~Warehouse(){Logger("Warehouse") << "DTOR" << std::endl;}
private:
    struct TransportHelper
    {
        explicit TransportHelper(unsigned transporterNr) : transporterNr(transporterNr)
        {}
//        bool operator ==(const TransportHelper& other) const {return this->transporterNr == other.transporterNr;}
//        bool operator <(const TransportHelper& other) const {return this->transporterNr < other.transporterNr;}
        const unsigned transporterNr;
        std::atomic<bool> isReady{false};
        ProductPtr product;
    };

    bool isTransportLoaded(unsigned transporterNr);
    auto getTransporterIterator(unsigned transporterNr);
    bool isTransporterWaiting(unsigned transporterNr);

    ThreadSafeQueue<OperationResultPtr> depot_;
    std::vector<std::unique_ptr<TransportHelper>> transportersToLoad_;
    unsigned maxSize_;
    std::shared_ptr<tools::IProgramStopControllerHelper> stopController_;
    std::mutex mutexForTransport_;
    std::condition_variable cv_;
};
