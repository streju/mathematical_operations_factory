#pragma once

#include <optional>

#include "Product.hpp"

struct CustomerRequest
{
    enum class Type
    {
        value,
        valueAndProductType,
        productType,
        any
    };
    CustomerRequest(unsigned customerNr) : customerNr(customerNr), type(Type::any){}
    CustomerRequest(unsigned customerNr, Type type, Product::Type productType)
        : customerNr(customerNr), type(type), productType(productType){}
    CustomerRequest(unsigned customerNr, Type type, double min, double max)
        : customerNr(customerNr), type(type), minValue(min), maxValue(max){}
    CustomerRequest(unsigned customerNr, Type type, Product::Type productType, double min, double max)
        : customerNr(customerNr), type(type), productType(productType), minValue(min), maxValue(max){}
    unsigned customerNr;
    Type type;
    std::optional<Product::Type> productType;
    std::optional<double> minValue;
    std::optional<double> maxValue;
};
