#pragma once

#include <memory>

struct Operation;

class IWorker
{
public:
    virtual void startProcessingOperation(const unsigned, const std::shared_ptr<Operation>&) = 0;
    virtual void loadTransport(unsigned) = 0;
    virtual void movePendingsOperationsToWarehouse(unsigned) = 0;
    virtual ~IWorker() = default;
};
