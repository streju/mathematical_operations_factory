#pragma once

#include <memory>

struct Product
{
    enum class Type
    {
        addition,
        substraction,
        multiplication,
        division,
    };

    Product(Type type, unsigned val) : type(type), value(val) {}
    const Type type;
    const double value;
};

using ProductPtr = std::shared_ptr<Product>;
