#pragma once

#include <functional>
#include <thread>

#include "Tools/ThreadSafeQueue.hpp"
#include "Tools/ThreadWrapper.hpp"
#include "WorkerHelper.hpp"

#include "Tools/IProgramStopControllerHelper.hpp"

class WorkQueue : public tools::ThreadSafeQueue<WorkerHelper>
{
public:
    WorkQueue(){}

    bool try_pop(WorkerHelper& result, std::function<bool(WorkerHelper&)> predicate)
    {
        std::unique_lock<std::mutex> lock(tools::ThreadSafeQueue<WorkerHelper>::mutex_);
        if (tools::ThreadSafeQueue<WorkerHelper>::queue_.empty())
        {
            return false;
        }

        if (!predicate(tools::ThreadSafeQueue<WorkerHelper>::queue_.front()))
        {
            result = tools::ThreadSafeQueue<WorkerHelper>::queue_.front();
            return true;
        }
        result = tools::ThreadSafeQueue<WorkerHelper>::queue_.front();
        tools::ThreadSafeQueue<WorkerHelper>::queue_.pop();
        return true;
    }
};

class WorkersPool
{
public:
    WorkersPool(const tools::ProgramStopControllerPtr& stopController, int nrOfThreads)
        : stopController_(stopController), nrOfThreads_(nrOfThreads)
    {}

    template <typename FuncType, typename ...Args>
    void pushToQueue(FuncType f, Args&&... args)
    {
        queue_.push(WorkerHelper(f, args...));

        // mutex not needed till only one thread(manager) can call this function
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
                task(threadNr);
            }
            else
            {
                std::this_thread::yield();
            }
        }
    }

    void startThreads()
    {
        for (int threadNr = 1; threadNr <= nrOfThreads_; ++threadNr)
        {
            threads_.emplace_back(std::make_unique<tools::ThreadWrapper>(
                std::thread(&WorkersPool::work, this, threadNr), true));
        }
    }

    const tools::ProgramStopControllerPtr stopController_;
    int nrOfThreads_;
    std::vector<std::unique_ptr<tools::ThreadWrapper>> threads_;
    WorkQueue queue_;
};
