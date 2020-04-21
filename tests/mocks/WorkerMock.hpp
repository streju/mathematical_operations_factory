#pragma once

#include <gmock/gmock.h>

#include "IWorker.hpp"

namespace mocks
{

class WorkerMock : public IWorker
{
public:
    MOCK_METHOD2(startProcessingOperation, void(const unsigned, const std::shared_ptr<Operation>&));
    MOCK_METHOD1(loadTransport, void(unsigned));
    MOCK_METHOD1(movePendingsOperationsToWarehouse, void(unsigned));
};

} // namespace mocks

