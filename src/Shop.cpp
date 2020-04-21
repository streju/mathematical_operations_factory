#include "Shop.hpp"

#include <algorithm>
#include "HelpersFunctions.hpp"

Shop::Shop()
{
}

ProductPtr Shop::buy(const CustomerRequest& req)
{
    ProductPtr product;
    std::scoped_lock<std::mutex> lk(mu_);

    if (goods_.empty()) return nullptr;

    switch (req.type)
    {
        case CustomerRequest::Type::any:
        {
            product = getFirstProduct();
            break;
        }
        case CustomerRequest::Type::value:
        {
            if (!req.maxValue || !req.minValue) return nullptr;
            product = getProductByPredicate([maxValue = req.maxValue, minValue = req.minValue](const auto& pr)
                {return pr->value >= minValue && pr->value <= maxValue;});
            break;
        }
        case CustomerRequest::Type::productType:
        {
            if (!req.productType) return nullptr;
            product = getProductByPredicate([type = req.productType](const auto& pr){return pr->type == type;});
            break;
        }
        case CustomerRequest::Type::valueAndProductType:
        {
            if (!req.maxValue || !req.minValue || !req.productType) return nullptr;
            product = getProductByPredicate([type = req.productType, maxValue = req.maxValue, minValue = req.minValue](const auto& pr)
                {return pr->type == type && pr->value >= minValue && pr->value <= maxValue;});
            break;
        }
    }
    if (product)
    {
        Logger(prefix_) << "Selling product: Type " << productTypeToString(product->type)
            << ", value " << product->value << "to customer nr " << req.customerNr << std::endl;
    }
    return product;
}

ProductPtr Shop::getFirstProduct()
{
    auto product = goods_.front();
    goods_.erase(goods_.begin());
    return product;
}

ProductPtr Shop::getProductByPredicate(const std::function<bool(const ProductPtr&)>& predicate)
{
    auto iter = std::find_if(goods_.begin(), goods_.end(), predicate);
    if (iter == goods_.end()) return nullptr;

    auto product = *iter;
    goods_.erase(iter);
    return product;
}

bool Shop::isOpen()
{
    std::scoped_lock<std::mutex> lk(mu_);
    return !goods_.empty();
}

void Shop::deliver(const ProductPtr& product)
{
    std::scoped_lock<std::mutex> lk(mu_);
    Logger(prefix_) << "New product: Type " << productTypeToString(product->type) << ", value " << product->value << std::endl;
    goods_.push_back(product);
}
