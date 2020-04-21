#include "Warehouse.hpp"

#include <algorithm>
#include <utility>
#include <thread>

#include "Tools/Randoms.hpp"

Product::Type translateOperResultToProduct(Operation::Type type)
{
    switch (type)
    {
        case Operation::Type::addition:
            return Product::Type::addition;
        case Operation::Type::substraction:
            return Product::Type::substraction;
        case Operation::Type::multiplication:
            return Product::Type::multiplication;
        case Operation::Type::division:
            return Product::Type::division;
    }
    Logger() << "Cannot translate Operation Type from Result to Product" << std::endl;
    return Product::Type::addition;
}

Warehouse::Warehouse(unsigned maxSize, const std::shared_ptr<tools::IProgramStopControllerHelper>& stopController)
    : maxSize_(maxSize) , stopController_(stopController)
{}

void Warehouse::put(const OperationResultPtr& operationResult)
{
    if (isFull())
    {
        Logger() << "Error. Try to put to full warehouse." << std::endl;
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

auto Warehouse::getTransporterIterator(unsigned transporterNr) //ogarnąć bez auto
{
    auto isEqual = [transporterNr](const std::unique_ptr<TransportHelper>& transport)
        {return transport->transporterNr == transporterNr;};
    auto it = std::find_if(transportersToLoad_.cbegin(), transportersToLoad_.cend(), isEqual);
    return it;
}

ProductPtr Warehouse::handleTransporterArrival(unsigned transporterNr)
{
    std::unique_lock<std::mutex> lk(mutexForTransport_);

    if (isTransporterWaiting(transporterNr))
    {
        Logger() << "Transporter nr " << transporterNr << " should not be already in queue!!" << std::endl;
        return nullptr;
    }
    transportersToLoad_.emplace_back(std::make_unique<TransportHelper>(transporterNr));

    Logger("Transporter") << "start WAIT" << std::endl;
    cv_.wait(lk, [this, transporterNr]()
    {
        Logger("Transporter") << "still WAIT" << std::endl;
        return isTransportLoaded(transporterNr) || stopController_->wasStopRequested();
    });

    if (stopController_->wasStopRequested()) return nullptr;

    auto it = getTransporterIterator(transporterNr);
    Logger() << "Transporter nr " << transporterNr << " can start." << std::endl;
    auto product = (*it)->product;
    transportersToLoad_.erase(it); // chyba nie erase;
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
    std::this_thread::sleep_for(std::chrono::seconds(tools::random(4, 10)));
    std::scoped_lock<std::mutex> lk(mutexForTransport_);
    if (transportersToLoad_.empty())
    {
        //error
        return;
    }
    auto& transport = transportersToLoad_.front();
    transport->isReady = true;
    OperationResultPtr result;
    if (!depot_.try_pop(result))
    {
        //error and remove from vector
        return;
    }
    transport->product = std::make_shared<Product>(translateOperResultToProduct(result->type_), result->value_);
    cv_.notify_all();

}

bool Warehouse::isTransportLoaded(unsigned transporterNr)
{
    if (isTransporterWaiting(transporterNr))
    {
        auto it = getTransporterIterator(transporterNr);
        return (*it)->isReady;
    }
    //error powinno być w kolejce
    return false;
}

bool Warehouse::isTransporterWaiting(unsigned transporterNr)
{
    return getTransporterIterator(transporterNr) != transportersToLoad_.end();
}
