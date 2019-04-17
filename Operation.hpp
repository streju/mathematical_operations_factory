#ifndef OPERATION_HPP
#define OPERATION_HPP

#include <iostream>
#include <mutex>
#include <condition_variable>

struct Operation
{

    enum class Type
    {
        addition,
        substraction,
        multiplication,
        division,
    };

    Operation(Type type, int operNr) : type_(type), operNr_(operNr), nrOfAssignedWorkers_(0), isReady_(false)
    {
        switch (type_)
        {
            case Type::addition:
            case Type::substraction:
                nrOfNeededWorkers_ = 1;
                break;
            case Type::multiplication:
            case Type::division:
                nrOfNeededWorkers_ = 2;
                break;
            default:
                std::cout << "Unsupported operation" << std::endl;
        }
    }

    Operation(const Operation& oper)
    {
        std::lock_guard<std::mutex> lock(oper.mutex_);
        type_ = oper.type_;
        nrOfNeededWorkers_ = oper.nrOfNeededWorkers_;
        nrOfAssignedWorkers_ = oper.nrOfAssignedWorkers_;
        operNr_ = oper.operNr_;
		isReady_ = oper.isReady_;
    }

    friend std::ostream& operator<<(std::ostream& os, Operation& oper)
    {
        switch (oper.type_)
        {
            case Operation::Type::addition:
                return os << oper.operNr_ << " addition";
            case Operation::Type::substraction:
                return os << oper.operNr_ << " substraction";
            case Operation::Type::multiplication:
                return os << oper.operNr_ << " multiplication";
            case Operation::Type::division:
                return os << oper.operNr_ << " division";
        }
        return os << "Operation not known";
    }

    void assignWorker()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        nrOfAssignedWorkers_++;
    }

    bool isEnoughWorkers()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return nrOfNeededWorkers_ == nrOfAssignedWorkers_;
    }

	void waitForCoworker()
	{
		std::unique_lock<std::mutex> lock(mutex_);
		cv_.wait(lock, [this] 
		{
			std::cout << "isReady_: " << isReady_ << std::endl;
			return isReady_;
		});
	}

	void notifyCoworker()
	{
		std::lock_guard<std::mutex> lock(mutex_);
        std::cout << "Notify other worker that operation: " << operNr_ << " can be processed" << std::endl;
		isReady_ = true;
		cv_.notify_all();
	}

	Type getOperationType()
	{
		return type_;
	}

private:
    Type type_;
    int nrOfNeededWorkers_;
    int operNr_;
    int nrOfAssignedWorkers_;
	bool isReady_;
    mutable std::mutex mutex_;
	std::condition_variable cv_;
};

#endif // OPERATION_HPP
