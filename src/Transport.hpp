#pragma once

#include <vector>
#include <thread>

#include "Tools/IProgramStopControllerHelper.hpp"
#include "Tools/ThreadWrapper.hpp"
#include "IWarehouseEntryPoint.hpp"
#include "Shop.hpp"

class Transport
{
public:
    Transport(const std::shared_ptr<tools::IProgramStopControllerHelper>& stopController,
        const std::shared_ptr<IWarehouseEntryPoint>& warehouse,
        const std::shared_ptr<IShop>& shop);
    ~Transport();
    void start(unsigned transporterNr);
private:
    void simulateWayBetweenWarehouseAndShop();
    const std::shared_ptr<tools::IProgramStopControllerHelper> stopController_;
    std::shared_ptr<IWarehouseEntryPoint> warehouse_;
    std::shared_ptr<IShop> shop_;
};
