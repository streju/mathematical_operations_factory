#pragma once

#include "IWarehouseEntryPoint.hpp"
#include "IShop.hpp"
#include "Tools/IProgramStopControllerHelper.hpp"

class Transport
{
public:
    Transport(const tools::ProgramStopControllerPtr& stopController,
        const WarehouseEntryPointPtr& warehouse,
        const ShopPtr& shop);
    void start(unsigned transporterNr);
private:
    void simulateWayBetweenWarehouseAndShop(const std::string& prefix);

    const tools::ProgramStopControllerPtr stopController_;
    WarehouseEntryPointPtr warehouse_;
    ShopPtr shop_;
};
