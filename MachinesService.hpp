#ifndef MACHINESSERVICE_HPP
#define MACHINESSERVICE_HPP

#include <map>
#include <string>
#include <utility>
#include <thread>
#include <vector>

#include "BaseOperationMachine.hpp"
#include "Operation.hpp"

struct MachineHelper
{
    MachineHelper(std::unique_ptr<IBaseOperationMachine>&& machine, unsigned int nr)
        : machine_(std::move(machine))
        , machineNr_(nr)
	{}

    unsigned int getNumber() const
    {
       return machineNr_;
    }
	std::unique_ptr<IBaseOperationMachine> machine_;
	std::thread thread_;
    unsigned int machineNr_;
};

class MachinesService
{
public:
    MachinesService();
    ~MachinesService();
    void act(std::shared_ptr<Operation> operation);
private:
    std::map<Operation::Type, std::vector<std::unique_ptr<MachineHelper>>> machines_;
    std::string prefix_;
};

#endif // MACHINESSERVICE_HPP
