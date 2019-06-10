#include "MachinesService.hpp"

#include "AdditionMachine.hpp"
#include "SubstractionMachine.hpp"
#include "MultiplicationMachine.hpp"
#include "DivisionMachine.hpp"

MachinesService::MachinesService() : prefix_("MachinesService: ")
{
    unsigned int totalNr = 0;
    unsigned int nrOfTypeMachines = rand() % 2 + 1;
    std::cout << prefix_ << "going to create " << nrOfTypeMachines << " addition machines" << std::endl;

    for (int i = 1; i <= nrOfTypeMachines; ++i)
	{
        machines_[Operation::Type::addition].emplace_back(
            std::make_unique<MachineHelper>(
                std::make_unique<AdditionMachine>(), totalNr + i));
	}
    totalNr += nrOfTypeMachines;
    nrOfTypeMachines = rand() % 2 + 1;
    std::cout << prefix_ << "going to create " << nrOfTypeMachines << " substraction machines" << std::endl;
    for (unsigned int i = 1; i <= nrOfTypeMachines; ++i)
	{
        machines_[Operation::Type::substraction].emplace_back(
            std::make_unique<MachineHelper>(
                std::make_unique<SubstractionMachine>(), totalNr + i));
	}

    totalNr += nrOfTypeMachines;
    nrOfTypeMachines = rand() % 2 + 1;
    std::cout << prefix_ << "going to create " << nrOfTypeMachines << " multiplication machines" << std::endl;
    for (int i = 1; i <= nrOfTypeMachines; ++i)
	{
        machines_[Operation::Type::multiplication].emplace_back(
            std::make_unique<MachineHelper>(
                std::make_unique<MultiplicationMachine>(), totalNr + i));
	}

    totalNr += nrOfTypeMachines;
    nrOfTypeMachines = rand() % 2 + 1;
    std::cout << prefix_ << "going to create " << nrOfTypeMachines << " division machines" << std::endl;
    for (int i = 1; i <= nrOfTypeMachines; ++i)
	{
        machines_[Operation::Type::division].emplace_back(
            std::make_unique<MachineHelper>(
                std::make_unique<DivisionMachine>(), totalNr + i));
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

void MachinesService::act(std::shared_ptr<Operation> operation)
{
    std::cout << prefix_ << " act" << std::endl;

    for (const auto& machine : machines_.at(operation->getOperationType()))
    {
        if (machine->machine_->isBusy())
        {
            std::cout << prefix_ << " machine nr " << machine->getNumber() << " is already in use" << std::endl;
        }
    }

}
