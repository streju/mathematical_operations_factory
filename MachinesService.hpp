#ifndef MACHINESSERVICE_HPP
#define MACHINESSERVICE_HPP

#include <map>
#include <string>
#include <utility>
#include <thread>

#include "BaseOperationMachine.hpp"
#include "Operation.hpp"

struct MachineHelper
{
	MachineHelper(std::unique_ptr<IBaseOperationMachine>&& machine) : machine_(std::move(machine))
	{}
	std::unique_ptr<IBaseOperationMachine> machine_;
	std::thread thread_;
};

class MachinesService
{
 public:
	 MachinesService();
	 ~MachinesService();
private:
	std::map<Operation::Type, std::vector<std::unique_ptr<MachineHelper>>> machines_;
	std::string prefix_;
};

#endif // MACHINESSERVICE_HPP
