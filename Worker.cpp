#include "Worker.hpp"

#include <iostream>
#include "src/Tools/Logger.hpp"

Worker::Worker(int nr, std::shared_ptr<ThreadSafeQueue> queue, std::shared_ptr<MachinesService> machine)
    : queue_(queue)
    , machine_(machine)
    , prefix_("Worker nr" + std::to_string(nr) + ": ")
{
    Logger() << prefix_ << "creation" << std::endl;
}

void Worker::tryAct()
{
	while (true)
	{
        Logger() << prefix_ << __func__ << std::endl;
		auto operationPtr = queue_->waitAndPop();
		if (!operationPtr->isEnoughWorkers())
		{
            Logger() << prefix_ << "elo" << std::endl;
			operationPtr->waitForCoworker();
		}
        Logger() << prefix_ << "got new operation " << *operationPtr << std::endl;

        transportToMachine(operationPtr);
        machine_->act(operationPtr);
	}
}

void Worker::transportToMachine(std::shared_ptr<Operation> operation) const
{
    std::this_thread::sleep_for(std::chrono::seconds(rand() % 5 + 2));
    Logger() << prefix_ << "Transport of " << *operation << " end" << std::endl;
}
