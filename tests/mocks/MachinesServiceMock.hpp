#pragma once

#include <future>

#include <gmock/gmock.h>

#include "IMachinesService.hpp"

namespace mocks
{

class MachinesServiceMock : public IMachinesService
{
public:
    MOCK_METHOD1(registerOperation,
        std::shared_future<OperationResultPtr>(std::shared_ptr<Operation> operation));
};

} // namespace mocks
