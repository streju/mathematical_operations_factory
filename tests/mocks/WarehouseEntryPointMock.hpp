#pragma once

#include <gmock/gmock.h>

#include "IWarehouseEntryPoint.hpp"

namespace mocks
{

class WarehouseEntryPointMock : public IWarehouseEntryPoint
{
public:
    MOCK_METHOD1(placeOperResult, void(const OperationResultPtr&));
    MOCK_METHOD1(notifyAboutNewTransporter, ProductPtr(unsigned));
    MOCK_METHOD0(getState, WarehouseRaport());
    MOCK_METHOD1(startLoadTransport, void(const std::string&));
    MOCK_METHOD0(movePendings, void());
};

} // namespace mocks
