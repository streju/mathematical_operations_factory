#include "Worker.hpp"

#include <iostream>

Worker::Worker(int nr, std::shared_ptr<ThreadSafeQueue> queue, std::shared_ptr<MachinesService> machine)
    : queue_(queue)
    , machine_(machine)
    , prefix_("Worker nr" + std::to_string(nr) + ": ")
{
    std::cout << prefix_ << "creation" << std::endl;
}

void Worker::tryAct()
{
	while (true)
	{
		std::cout << prefix_ << __func__ << std::endl;
		auto operationPtr = queue_->waitAndPop();
		if (!operationPtr->isEnoughWorkers())
		{
			std::cout << prefix_ << "elo" << std::endl;
			operationPtr->waitForCoworker();
		}
		std::cout << prefix_ << "got new operation " << *operationPtr << std::endl;

        transportToMachine(operationPtr);
        machine_->act(operationPtr);
	}
}

void Worker::transportToMachine(std::shared_ptr<Operation> operation) const
{
    std::this_thread::sleep_for(std::chrono::seconds(rand() % 5 + 2));
    std::cout << prefix_ << "Transport of " << *operation << " end" << std::endl;
}
