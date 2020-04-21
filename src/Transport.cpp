#include "Transport.hpp"

#include "Tools/Randoms.hpp"

Transport::Transport(const std::shared_ptr<tools::IProgramStopControllerHelper>& stopController,
    const std::shared_ptr<IWarehouseEntryPoint>& warehouse,
    const std::shared_ptr<IShop>& shop)
    : stopController_(stopController), warehouse_(warehouse) , shop_(shop)
{
}

Transport::~Transport()
{
    Logger("Transport") << "DTOR" << std::endl;
}

void Transport::start(unsigned transporterNr)
{
    while (!stopController_->wasStopRequested())
    {
        std::this_thread::sleep_for(std::chrono::seconds(tools::random(5, 10)));
        Logger() << "Transporter nr " << transporterNr << " will arive to warehouse" << std::endl;
        auto product = warehouse_->notifyAboutNewTransporter(transporterNr);
        if (!product)
        {
            Logger() << "Critical error! Transport loading shoul never fail.";
//            std::abort();
            continue;
        }
        simulateWayBetweenWarehouseAndShop();
        shop_->deliver(product);
    }
}

void Transport::simulateWayBetweenWarehouseAndShop()
{
    std::this_thread::sleep_for(std::chrono::seconds(tools::random(5, 10)));
}
