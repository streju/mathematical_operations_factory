#pragma once

#include <array>
#include <optional>
#include <memory>

#include "CustomerRequest.hpp"
#include "Product.hpp"
#include "IShop.hpp"
#include "Tools/IProgramStopControllerHelper.hpp"
#include "Tools/Logger.hpp"


class Customer
{
public:
    Customer(const tools::ProgramStopControllerPtr& stopController, const ShopPtr& shop);

    void start(unsigned threadNr);
private:

    std::optional<CustomerRequest> prepareRequest(const std::string& prefix, unsigned threadNr) const;
    std::string printRequest(CustomerRequest req) const;

    const tools::ProgramStopControllerPtr stopController_;
    const ShopPtr shop_;
    const std::array<CustomerRequest::Type, 4> reqTypes_{
        CustomerRequest::Type::value, CustomerRequest::Type::valueAndProductType, CustomerRequest::Type::productType, CustomerRequest::Type::any};
    const std::array<Product::Type, 4> possibleProductTypes_{
        Product::Type::addition, Product::Type::substraction, Product::Type::multiplication, Product::Type::division};
    int minValueLimit_{70};
    int maxValueLimit_{10000};
};
