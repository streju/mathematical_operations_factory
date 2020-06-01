#include "Transport.hpp"

#include <thread>

#include "Tools/Randoms.hpp"
#include "Tools/Timer.hpp"

Transport::Transport(const tools::ProgramStopControllerPtr& stopController,
    const WarehouseEntryPointPtr& warehouse,
    const ShopPtr& shop)
    : stopController_(stopController), warehouse_(warehouse) , shop_(shop)
{
}

void Transport::start(unsigned transporterNr)
{
    std::string prefix{"Transporter nr" + std::to_string(transporterNr) + ": "};

    while (!stopController_->wasStopRequested())
    {
        simulateWayBetweenWarehouseAndShop(prefix);

        Logger(prefix) << "Will arive to warehouse" << std::endl;
        auto product = warehouse_->notifyAboutNewTransporter(transporterNr);
        if (!product)
        {
            if (stopController_->wasStopRequested()) return;

            Logger(prefix) << "Critical error! Transport loading failed.";
            std::abort();
            continue;
        }

        simulateWayBetweenWarehouseAndShop(prefix);
        shop_->deliver(product);
    }
}

void Transport::simulateWayBetweenWarehouseAndShop(const std::string& prefix)
{
    tools::SecondsTimer timer;
    std::this_thread::sleep_for(std::chrono::seconds(tools::random(5, 10)));
    Logger(prefix) << "Traveled the road between warehouse and shop in " << timer.stopAndGetTime() << "s." << std::endl;
}
