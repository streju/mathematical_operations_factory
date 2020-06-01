#pragma once

#include <string>

#include "Operation.hpp"
#include "Product.hpp"

std::string productTypeToString(Product::Type type);
Product::Type operationTypeToProductType(Operation::Type type);
