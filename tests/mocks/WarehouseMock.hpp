#pragma once

#include <gmock/gmock.h>

#include "IWarehouse.hpp"

namespace mocks
{

class WarehouseMock : public IWarehouse
{
public:
    MOCK_METHOD1(put, void(const OperationResultPtr&));
    MOCK_METHOD1(handleTransporterArrival, ProductPtr(unsigned));
    MOCK_METHOD0(isFull, bool());
    MOCK_METHOD0(isAnyTransporterWaiting, bool());
    MOCK_METHOD1(startLoadTransport, void(const std::string&));
};

} // namespace mocks
