#pragma once

#include <string>

#include "Product.hpp"
#include "OperationResult.hpp"

struct WarehouseRaport
{
    WarehouseRaport(bool isDepotFull, bool isAnyTransporterWaiting, bool isAnyOperationPending)
        : isDepotFull(isDepotFull), isAnyTransporterWaiting(isAnyTransporterWaiting), isAnyOperationPending(isAnyOperationPending)
    {}
    bool isDepotFull;
    bool isAnyTransporterWaiting;
    bool isAnyOperationPending;
};

class IWarehouseEntryPoint
{
public:
    virtual void placeOperResult(const OperationResultPtr& operationResult) = 0;
    virtual ProductPtr notifyAboutNewTransporter(unsigned transporterNr) = 0;
    virtual void startLoadTransport(const std::string& prefix) = 0;
    virtual WarehouseRaport getState() = 0;
    virtual void movePendings() = 0;
    virtual ~IWarehouseEntryPoint() = default;
};

using WarehouseEntryPointPtr = std::shared_ptr<IWarehouseEntryPoint>;
