#include "Worker.hpp"

#include <iostream>

Worker::Worker(int nr, std::shared_ptr<ThreadSafeQueue> queue)
    : queue_(queue)
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

		std::this_thread::sleep_for(std::chrono::seconds(rand() % 5 + 2));
	}
}
