#pragma once

#include <gmock/gmock.h>

#include "IShop.hpp"

namespace mocks
{

class ShopMock : public IShop
{
public:
    MOCK_METHOD1(buy, ProductPtr(const CustomerRequest&));
    MOCK_METHOD0(isOpen, bool());
    MOCK_METHOD1(deliver, void(const ProductPtr&));
};

} // namespace mocks
