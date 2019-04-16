#ifndef THREADSAFEQUEUE_HPP
#define THREADSAFEQUEUE_HPP

#include <iostream>

#include <condition_variable>
#include <queue>
#include <memory>

#include <mutex>
#include <thread>

#include "Operation.hpp"

class ThreadSafeQueue
{
public:
    ThreadSafeQueue()
    {
    }

    void push(std::shared_ptr<Operation>& dataPtr)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(dataPtr);
        cv_.notify_one();
    }

    std::shared_ptr<Operation> waitAndPop()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this](){
            std::cout << "Notification!" << std::endl;
            return !queue_.empty(); /*&& warunek czy wystarczy wątków*/
        });
        auto result = queue_.front();
        result->assignWorker();

        if (!result->isEnoughWorkers())
        {
			return result;
        }
		else if (result->getOperationType() == Operation::Type::multiplication ||
			result->getOperationType() == Operation::Type::division)
		{
			std::cout << "Try to notify coworker" << std::endl;
			result->notifyCoworker();
		}

        queue_.pop();
        return result;
    }

private:
    std::queue<std::shared_ptr<Operation>> queue_;
    std::condition_variable cv_;
    std::mutex mutex_;
};

#endif // THREADSAFEQUEUE_HPP
