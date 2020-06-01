#pragma once

#include <functional>
#include <mutex>
#include <vector>

#include "IShop.hpp"

class Shop : public IShop
{
public:
    Shop();

    ProductPtr buy(const CustomerRequest& req) override;
    bool isOpen() override;
    void deliver(const ProductPtr& product) override;

private:
    ProductPtr getProductByPredicate(const std::function<bool(const ProductPtr&)>& predicate);
    ProductPtr getFirstProduct();

    std::vector<ProductPtr> goods_;
    std::mutex mu_;
    std::string prefix_{"Shop: "};
};
