#ifndef OPERATION_HPP
#define OPERATION_HPP

#include <iostream>
#include <mutex>

struct Operation
{

    enum class Type
    {
        addition,
        substraction,
        multiplication,
        division,
    };

    Operation(Type type, int operNr) : type_(type), operNr_(operNr), nrOfAssignedWorkers_(0)
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
    }

    friend std::ostream& operator<<(std::ostream& os, Operation& oper)
    {
        switch (oper.type_)
        {
            case Operation::Type::addition:
                return os << "addition";
            case Operation::Type::substraction:
                return os << "substraction";
            case Operation::Type::multiplication:
                return os << "multiplication";
            case Operation::Type::division:
                return os << "division";
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

private:
    Type type_;
    int nrOfNeededWorkers_;
    int operNr_;
    int nrOfAssignedWorkers_;
    mutable std::mutex mutex_;
};

#endif // OPERATION_HPP
