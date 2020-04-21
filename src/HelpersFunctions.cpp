#include "HelpersFunctions.hpp"

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
