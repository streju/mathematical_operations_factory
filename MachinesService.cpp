#include "MachinesService.hpp"

#include "AdditionMachine.hpp"
#include "SubstractionMachine.hpp"
#include "MultiplicationMachine.hpp"
#include "DivisionMachine.hpp"

MachinesService::MachinesService() : prefix_("MachinesService: ")
{
	int nrOfMachines = rand() % 2 + 1;
	std::cout << prefix_ << "going to create " << nrOfMachines << " addition machines" << std::endl;

	for (int i = 0; i < nrOfMachines; ++i)
	{
		machines_[Operation::Type::addition].emplace_back(std::make_unique<MachineHelper>(std::make_unique<AdditionMachine>()));
	}

	nrOfMachines = rand() % 2 + 1;
	std::cout << prefix_ << "going to create " << nrOfMachines << " substraction machines" << std::endl;
	for (int i = 0; i < nrOfMachines; ++i)
	{
		machines_[Operation::Type::substraction].emplace_back(std::make_unique<MachineHelper>(std::make_unique<SubstractionMachine>()));
	}

	nrOfMachines = rand() % 2 + 1;
	std::cout << prefix_ << "going to create " << nrOfMachines << " multiplication machines" << std::endl;
	for (int i = 0; i < nrOfMachines; ++i)
	{
		machines_[Operation::Type::multiplication].emplace_back(std::make_unique<MachineHelper>(std::make_unique<MultiplicationMachine>()));
	}

	nrOfMachines = rand() % 2 + 1;
	std::cout << prefix_ << "going to create " << nrOfMachines << " division machines" << std::endl;
	for (int i = 0; i < nrOfMachines; ++i)
	{
		machines_[Operation::Type::division].emplace_back(std::make_unique<MachineHelper>(std::make_unique<DivisionMachine>()));
	}
}

MachinesService::~MachinesService()
{
	for (auto& typeToMachine : machines_)
	{
		for (auto& machineHelper : typeToMachine.second)
		{
			machineHelper->thread_.join();
		}
	}
}