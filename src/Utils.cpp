#include "Utils.hpp"

#include "Tools/Logger.hpp"

std::string productTypeToString(Product::Type type)
{
    switch (type)
    {
        case Product::Type::addition:
            return "addition";
        case Product::Type::substraction:
            return "substraction";
        case Product::Type::multiplication:
            return "multiplication";
        case Product::Type::division:
            return "division";
    }
    return "Product type not known";
}

Product::Type operationTypeToProductType(Operation::Type type)
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
    Logger() << "ERROR: Cannot translate Operation Type to Product Type." << std::endl;
    return Product::Type::addition;
}
