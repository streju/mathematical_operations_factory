#pragma once

#include <atomic>
#include <thread>
#include <future>
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
        virtual ~impl_base(){}
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
    ThreadPool(const std::shared_ptr<tools::IProgramStopControllerHelper>& stopController, int nrOfThreads)
        : stopController_{stopController}
    {
        for (int threadNr = 1; threadNr <= nrOfThreads; ++threadNr)
        {
            threads_.emplace_back(std::make_unique<ThreadWrapper>(
                std::thread(&ThreadPool::work, this, threadNr), true));
        }
    }

    virtual ~ThreadPool()
    {
        Logger("ThreadsPool") << "DTOR" << std::endl;
    }

//    template <typename FuncType, typename ...Args>
//    void pushToQueue(FuncType&& f, Args&&... args)
//    {
//        std::function<decltype(f(args...))()> func = std::bind(std::forward<FuncType>(f), std::forward<Args>(args)...);
//        std::function<void(int)> functionWrapper = [func](int threadNr){func(threadNr);};
//        queue_.push(functionWrapper);
//    }

//    template <typename FuncType>
//    void pushToQueue(FuncType f)
//    {
//        queue_.push(std::move(f));
//    }

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
        Logger("ThreadsPool") << "STOP REQUESTED" << std::endl;
    }
    const std::shared_ptr<tools::IProgramStopControllerHelper> stopController_;
    std::vector<std::unique_ptr<ThreadWrapper>> threads_;
    ThreadSafeQueue<FunctionWrapper> queue_;
};

} //namespace tools
