#pragma once

#include <functional>
#include <optional>
#include <thread>
#include <vector>

#include "CustomerRequest.hpp"
#include "IShop.hpp"
#include "Operation.hpp"
#include "Product.hpp"

class Shop : public IShop
{
public:
    Shop();

    ProductPtr buy(const CustomerRequest& req) override;
    bool isOpen() override;
    void deliver(const ProductPtr& product) override;

    ~Shop(){Logger(prefix_) << "DTOR" << std::endl;}

private:
    ProductPtr getProductByPredicate(const std::function<bool(const ProductPtr&)>& predicate);
    ProductPtr getFirstProduct();

    std::vector<ProductPtr> goods_;
    std::mutex mu_;
    std::string prefix_{"Shop: "};
};
