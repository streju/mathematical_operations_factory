#include "Warehouse.hpp"

#include <algorithm>
#include <utility>
#include <thread>

#include "Tools/Randoms.hpp"
#include "Tools/Timer.hpp"
#include "Utils.hpp"

Warehouse::Warehouse(unsigned maxSize, const tools::ProgramStopControllerPtr& stopController)
    : maxSize_(maxSize) , stopController_(stopController)
{}

void Warehouse::put(const OperationResultPtr& operationResult)
{
    if (isFull())
    {
        Logger() << "Error. Try to put when warehouse is full." << std::endl;
        std::abort();
        return;
    }
    depot_.push(operationResult);
    Logger() << "Putting OperationResult nr " << operationResult->operationNr_ << " to warehouse end." << std::endl;
}

bool Warehouse::isFull()
{
   return depot_.size() == maxSize_;
}

ProductPtr Warehouse::handleTransporterArrival(unsigned transporterNr)
{
    std::unique_lock<std::mutex> lk(mutexForTransport_);

    if (isTransporterWaiting(transporterNr))
    {
        Logger() << "Transporter nr " << transporterNr << " should not be already in queue!!" << std::endl;
        return nullptr;
    }

    transportersToLoad_.emplace(std::piecewise_construct,
        std::forward_as_tuple(transporterNr),
        std::forward_as_tuple());

    cv_.wait(lk, [this, transporterNr]()
    {
        return isTransportLoaded(transporterNr) || stopController_->wasStopRequested();
    });

    if (stopController_->wasStopRequested()) return nullptr;

    if (!isTransporterWaiting(transporterNr))
    {
        Logger() << "Critical error transporter " << transporterNr << " should be in queue" << std::endl;
        std::abort();
        return nullptr;
    }

    Logger() << "Transporter nr " << transporterNr << " can start." << std::endl;
    auto product = transportersToLoad_[transporterNr].product;
    transportersToLoad_.erase(transporterNr);
    return product;
}

bool Warehouse::isAnyTransporterWaiting()
{
    std::scoped_lock<std::mutex> lk(mutexForTransport_);
    return !transportersToLoad_.empty();
}

void Warehouse::startLoadTransport(const std::string& prefix)
{
    Logger(prefix) << "Start loading transport" << std::endl;
    tools::SecondsTimer timer;
    std::this_thread::sleep_for(std::chrono::seconds(tools::random(4, 10)));

    std::scoped_lock<std::mutex> lk(mutexForTransport_);

    if (transportersToLoad_.empty())
    {
        Logger(prefix) << "There are no waiting transporters to load." << std::endl;
        return;
    }

    OperationResultPtr result;
    if (!depot_.try_pop(result))
    {
        Logger(prefix) << "There are no operations results in depot!" << std::endl;
        return;
    }

    auto transport = transportersToLoad_.begin();
    (*transport).second.product = std::make_shared<Product>(operationTypeToProductType(result->type_), result->value_);
    (*transport).second.isReady = true;

    Logger(prefix) << "Notify waiting transporters. Transporter loaded in "
        << timer.stopAndGetTime() << "s." << std::endl;

    cv_.notify_all();
}

bool Warehouse::isTransportLoaded(unsigned transporterNr)
{
    if (isTransporterWaiting(transporterNr))
    {
        return transportersToLoad_[transporterNr].isReady;
    }
    return false;
}

bool Warehouse::isTransporterWaiting(unsigned transporterNr)
{
    return transportersToLoad_.find(transporterNr) != transportersToLoad_.end();
}
