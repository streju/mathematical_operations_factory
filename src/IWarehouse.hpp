#pragma once

#include <memory>

#include "Product.hpp"
#include "OperationResult.hpp"

class IWarehouse
{
public:
    enum class State
    {
        DeliveryPossibleTransporterWaits,
        DeliveryPossibleLackOfTransporters,
        FullAndLackOfTransporters,
        FullAndTransportWaits
    };

    virtual void put(const OperationResultPtr& operationResult) = 0;
    virtual ProductPtr handleTransporterArrival(unsigned transporterNr) = 0;
    virtual void startLoadTransport(const std::string& prefix) = 0;
    virtual bool isFull() = 0;
    virtual bool isAnyTransporterWaiting() = 0;
    virtual ~IWarehouse() = default;
};
