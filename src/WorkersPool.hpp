#pragma once

#include <atomic>
#include <functional>
#include <thread>

#include "../SafeQueue.hpp"
#include "ThreadWrapper.hpp"

class WorkersPool
{
public:
    WorkersPool(int nrOfWorkers) : done_{false}
    {
//        threads_.reserve(nrOfWorkers);
        for (int i = 0; i < nrOfWorkers; ++i)
        {
            threads_.emplace_back(std::make_unique<ThreadWrapper>(
                std::thread(&WorkersPool::work, this), true));
        }
    }
    ~WorkersPool()
    {
        done_ = true;
    }

    template <typename Func, typename ...Args>
    void pushToQueue(Func&& f, Args&&... args)
    {
        std::function<decltype(f(args...))()> func = std::bind(std::forward<Func>(f), std::forward<Args>(args)...);
        std::function<void()> functionWrapper = [func](){func();};
        queue_.push(functionWrapper);
    }

    template <typename Func>
    void pushToQueue(Func f)
    {
        queue_.push(std::function<void()>(f));
    }

private:
    void work()
    {
        while (!done_)
        {
            std::function<void()> task;
            if (queue_.try_pop(task))
            {
                task();
            }
            else
            {
                std::this_thread::yield();
            }
        }
    }
    std::vector<std::unique_ptr<ThreadWrapper>> threads_;
    SafeQueue<std::function<void()>> queue_;
    std::atomic<bool> done_;
};

