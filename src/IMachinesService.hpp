#ifndef IMACHINESSERVICE_HPP
#define IMACHINESSERVICE_HPP

#include <future>

struct Operation;
struct OperationResult;

class IMachinesService
{
public:
    virtual std::shared_future<std::shared_ptr<OperationResult>> registerOperation(std::shared_ptr<Operation> operation) = 0;
    virtual ~IMachinesService() = default;
};

using MachinesServicePtr = std::shared_ptr<IMachinesService>;

#endif // IMACHINESSERVICE_HPP
