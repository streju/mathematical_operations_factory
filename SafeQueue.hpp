#ifndef THREADSAFEQUEUE_HPP
#define THREADSAFEQUEUE_HPP

#include <iostream>

#include <condition_variable>
#include <queue>
#include <memory>

#include <mutex>
#include <thread>

#include "Operation.hpp"
#include "src/Tools/Logger.hpp"

class ThreadSafeQueue
{
public:
    ThreadSafeQueue()
    {
    }

    void push(std::shared_ptr<Operation>& dataPtr)
    {
        std::scoped_lock<std::mutex> lock(mutex_);
        queue_.push(dataPtr);
        cv_.notify_one();
    }

    std::shared_ptr<Operation> waitAndPop()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this](){
            Logger() << "Notification!" << std::endl;
            return !queue_.empty(); /*&& warunek czy wystarczy wątków*/
        });
        auto result = queue_.front();
        result->assignWorker();

        if (!result->isEnoughWorkers())
        {
            cv_.notify_one();
			return result;
        }
		else if (result->getOperationType() == Operation::Type::multiplication ||
			result->getOperationType() == Operation::Type::division)
		{
            Logger() << "Try to notify coworker" << std::endl;
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

template<typename T>
class SafeQueue
{
public:
    SafeQueue()
    {
    }

    void push(T& dataPtr)
    {
        std::scoped_lock<std::mutex> lock(mutex_);
        queue_.push(dataPtr);
        cv_.notify_one();
    }

    void wait_and_pop(T& result)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this] {return !queue_.empty();});
        result = std::move(queue_.front());
        queue_.pop();
    }

    virtual bool try_pop(T& result)
    {
        std::scoped_lock<std::mutex> lock(mutex_);
        if (queue_.empty())
        {
            return false;
        }
        result = std::move(queue_.front());
        queue_.pop();
        return true;
    }

private:
    std::queue<T> queue_;
    std::condition_variable cv_;
    std::mutex mutex_;
};

//class OperationQueue : public SafeQueue<std::shared_ptr<Operation>>
//{
//    OperationQueue()
//    {
//    }

//    bool try_pop(std::shared_ptr<Operation>& operation) override
//    {

//        std::scoped_lock<std::mutex> lock(mutex_);
//        if (queue_.empty())
//        {
//            return false;
//        }
//        result = queue_.front();
//        queue_.pop();
//        return true;
//    }

//private:
//    std::queue<std::shared_ptr<Operation>> queue_;
//    std::condition_variable cv_;
//    std::mutex mutex_;
//};

#endif // THREADSAFEQUEUE_HPP
