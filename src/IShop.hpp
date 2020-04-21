#pragma once

#include "Product.hpp"
#include "CustomerRequest.hpp"

class IShop
{
public:
    virtual ProductPtr buy(const CustomerRequest&) = 0;
    virtual bool isOpen() = 0;
    virtual void deliver(const ProductPtr&) = 0;
    virtual ~IShop() = default;
};
