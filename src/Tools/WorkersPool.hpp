#pragma once

#include <atomic>
#include <functional>
#include <thread>
#include <future>
#include <condition_variable>
#include <optional>

#include "ThreadSafeQueue.hpp"
#include "ThreadWrapper.hpp"
#include "ThreadsPool.hpp"
#include "../Operation.hpp"
#include "../Worker.hpp"
#include "WorkerHelper.hpp"


#include "IProgramStopControllerHelper.hpp"

template<typename T>
class WorkQueue : public ThreadSafeQueue<T>
{
public:
    WorkQueue()
    {
    }

//    void push(T data) //ogarnąć
//    {
//        std::scoped_lock<std::mutex> lock(mutex_);
//        queue_.push(std::move(data));
//        cv_.notify_one();
//    }

    bool try_pop(T& result, std::function<bool(T&)> predicate)
    {
        std::unique_lock<std::mutex> lock(ThreadSafeQueue<T>::mutex_);
        if (ThreadSafeQueue<T>::queue_.empty())
        {
            return false;
        }

        Logger() << "WorkQueue size: " << ThreadSafeQueue<T>::queue_.size()<< std::endl;
        if (!predicate(ThreadSafeQueue<T>::queue_.front()))
        {
            result = ThreadSafeQueue<T>::queue_.front();
            return true;
        }
        result = ThreadSafeQueue<T>::queue_.front();
        ThreadSafeQueue<T>::queue_.pop();
        return true;
    }
//protected:
//    std::queue<T> queue_;
//    std::condition_variable cv_;
//    std::mutex mutex_;
};

namespace tools
{

class WorkersPool
{
public:
    WorkersPool(const std::shared_ptr<tools::IProgramStopControllerHelper>& stopController, int nrOfThreads)
        : stopController_(stopController), nrOfThreads_(nrOfThreads)
    {}
    ~WorkersPool(){Logger("WokersPool") << "DTOR" << std::endl;}

    template <typename FuncType, typename ...Args>
    void pushToQueue(FuncType f, Args&&... args)
    {
        queue_.push(WorkerHelper(f, args...));

        // mutex not needed, because only one thread(manager) can call this function
        if (threads_.empty())
        {
            startThreads();
        }
    }

protected:
    void work(int threadNr)
    {
        Logger("WorkerNr" + std::to_string(threadNr) + " ") << "start working" << std::endl;
        while (!stopController_->wasStopRequested())
        {
            WorkerHelper task;
            auto predicate = [](WorkerHelper& wh){return wh.assignAndCheck();};
            if (queue_.try_pop(task, predicate))
            {
                Logger() << "Thread nr: " << threadNr << " executing task: " << std::endl;
                task(threadNr);
            }
            else
            {
                std::this_thread::yield();
            }
        }
        Logger("WorkerNr" + std::to_string(threadNr) + " ") << "stop was requested" << std::endl;

    }

    void startThreads()
    {
        for (int threadNr = 1; threadNr <= nrOfThreads_; ++threadNr)
        {
            threads_.emplace_back(std::make_unique<ThreadWrapper>(
                std::thread(&WorkersPool::work, this, threadNr), true));
        }
    }

    const std::shared_ptr<tools::IProgramStopControllerHelper> stopController_;
    int nrOfThreads_;
    std::vector<std::unique_ptr<ThreadWrapper>> threads_;
    WorkQueue<WorkerHelper> queue_;
};

} //namespace tools

