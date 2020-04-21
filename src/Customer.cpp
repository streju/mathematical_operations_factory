#include <thread>

#include "Customer.hpp"
#include "Tools/Randoms.hpp"
#include "HelpersFunctions.hpp"

Customer::Customer(
    const std::shared_ptr<tools::IProgramStopControllerHelper>& stopController, const std::shared_ptr<IShop>& shop)
    : stopController_(stopController), shop_(shop)
{
}

void Customer::start(unsigned threadNr)
{
    std::string prefix{"Customer nr" + std::to_string(threadNr) + ": "};
    std::this_thread::sleep_for(std::chrono::seconds(tools::random(2, 11)));
    while(!stopController_->wasStopRequested())
    {
        std::this_thread::sleep_for(std::chrono::seconds(tools::random(2, 11)));
        if (shop_->isOpen())
        {
            const auto request = prepareRequest(prefix, threadNr);
            if (request)
            {
                const auto product = shop_->buy(*request);
                if (product)
                {
                    Logger(prefix) << "Successfully buy product type: " << productTypeToString(product->type)
                        << " and value: " << product->value << std::endl;
                }
                else
                {
                    Logger(prefix) << "Unable to buy product: " << printRequest(*request) << std::endl;
                }
            }
        }
        else
        {
            Logger(prefix) << "Shop is closed! " << std::endl;
        }
    }
}

std::optional<CustomerRequest> Customer::prepareRequest(const std::string& prefix, unsigned threadNr) const
{
    std::optional<CustomerRequest> req;
    const auto typeOfRequest = tools::select_randomly_from_container(reqTypes_.begin(), reqTypes_.end());
    if (*typeOfRequest == CustomerRequest::Type::value)
    {
        const auto minValue = tools::random(0, minValueLimit_);
        const auto maxValue = tools::random(minValueLimit_, maxValueLimit_);
        Logger(prefix) << "Going to the shop with the intention of buying product with value between "
            << minValue << "-" << maxValue << std::endl;
        req = CustomerRequest(threadNr, *typeOfRequest, minValue, maxValue);
    }
    else if (*typeOfRequest == CustomerRequest::Type::productType)
    {
        const auto productType = tools::select_randomly_from_container(possibleProductTypes_.begin(), possibleProductTypes_.end());
        Logger(prefix) << "Going to the shop with the intention of buying product of operation " << productTypeToString(*productType) << std::endl;
        req = CustomerRequest(threadNr, *typeOfRequest, *productType);
    }
    else if (*typeOfRequest == CustomerRequest::Type::valueAndProductType)
    {
        const auto minValue = tools::random(0, minValueLimit_);
        const auto maxValue = tools::random(minValueLimit_, maxValueLimit_);
        const auto productType = tools::select_randomly_from_container(possibleProductTypes_.begin(), possibleProductTypes_.end());
        Logger(prefix) << "Going to the shop with the intention of buying product of operation "
            << productTypeToString(*productType) << ", witch value between" << minValue << "-" << maxValue << std::endl;
        req = CustomerRequest(threadNr, *typeOfRequest, *productType, minValue, maxValue);
    }
    else
    {
        Logger(prefix) << "Going to the shop with the intention of buying any product." << std::endl;
        req = CustomerRequest(threadNr);
    }
    return req;
}

std::string Customer::printRequest(CustomerRequest req) const
{
    std::string str("Type: ");
    if (req.productType) str += productTypeToString(*req.productType);
    else str += "none";

    str += ", minValue: ";
    if (req.minValue) str += std::to_string(*req.minValue);
    else str += "none";

    str += ", maxValue: ";
    if (req.maxValue) str += std::to_string(*req.maxValue);
    else str += "none";

    return str;
}
