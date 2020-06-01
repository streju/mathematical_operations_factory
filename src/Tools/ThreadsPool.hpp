#pragma once

#include <atomic>
#include <future>
#include <thread>
#include <type_traits>

#include "ThreadSafeQueue.hpp"
#include "ThreadWrapper.hpp"
#include "IProgramStopControllerHelper.hpp"

namespace tools
{

class FunctionWrapper
{
private:
    struct impl_base
    {
        virtual void call(int nr) = 0;
        virtual ~impl_base() = default;
    };
    template <typename FuncType>
    struct impl : public impl_base
    {
        FuncType f_;
        impl(FuncType&& f) : f_(std::move(f)) {}
        void call(int nr) override
        {
            f_(nr);
        }
    };
public:
    FunctionWrapper() = default;
    template<typename FuncType>
    FunctionWrapper(FuncType&& f) : impl_(std::make_shared<impl<FuncType>>(std::move(f)))
    {}

    void operator ()(int nr) {impl_->call(nr);}

    FunctionWrapper(FunctionWrapper&& other) : impl_(other.impl_)
    {}
    FunctionWrapper& operator=(FunctionWrapper&& other)
    {
        impl_ = other.impl_;
        return *this;
    }
    FunctionWrapper(const FunctionWrapper& other) : impl_(other.impl_)
    {}
    FunctionWrapper& operator=(const FunctionWrapper& other)
    {
        impl_ = other.impl_;
        return *this;
    }
private:
    std::shared_ptr<impl_base> impl_;
};

class ThreadPool
{
public:
    ThreadPool(const tools::ProgramStopControllerPtr& stopController, int nrOfThreads)
        : stopController_{stopController}
    {
        for (int threadNr = 1; threadNr <= nrOfThreads; ++threadNr)
        {
            threads_.emplace_back(std::make_unique<ThreadWrapper>(
                std::thread(&ThreadPool::work, this, threadNr), true));
        }
    }

    template <typename FuncType>
    std::future<typename std::result_of<FuncType(int)>::type> pushToQueue(FuncType f)
    {
        typedef typename std::result_of<FuncType(int)>::type result_type;
        std::packaged_task<result_type(int)> task(std::move(f));
        std::future<result_type> res = task.get_future();
        queue_.push(std::move(task));
        return res;
    }

protected:
    void work(int threadNr)
    {
        while (!stopController_->wasStopRequested())
        {
            FunctionWrapper task;
            if (queue_.try_pop(task))
            {
                task(threadNr);
            }
            else
            {
                std::this_thread::yield();
            }
        }
    }
    const tools::ProgramStopControllerPtr stopController_;
    std::vector<std::unique_ptr<ThreadWrapper>> threads_;
    ThreadSafeQueue<FunctionWrapper> queue_;
};

} //namespace tools
