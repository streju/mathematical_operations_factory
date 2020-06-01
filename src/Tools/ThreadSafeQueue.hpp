#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

namespace tools {

template<typename T>
class ThreadSafeQueue
{
public:
    ThreadSafeQueue() = default;

    void push(T data)
    {
        std::scoped_lock<std::mutex> lock(mutex_);
        queue_.push(std::move(data));
        cv_.notify_one();
    }

    void wait_and_pop(T& result)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this] {return !queue_.empty();});
        result = std::move(queue_.front());
        queue_.pop();
    }

    bool try_pop(T& result)
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

    bool empty()
    {
        std::scoped_lock<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    size_t size()
    {
        std::scoped_lock<std::mutex> lock(mutex_);
        return queue_.size();
    }
protected:
    std::queue<T> queue_;
    std::condition_variable cv_;
    std::mutex mutex_;
};

} // namespace tools
