#ifndef OPERATION_HPP
#define OPERATION_HPP

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>

#include "Tools/Logger.hpp"
#include "Tools/Randoms.hpp"

struct Operation
{

    enum class Type
    {
        addition,
        substraction,
        multiplication,
        division,
    };

    Operation(Type type, unsigned operNr) : type_(type), operNr_(operNr), nrOfAssignedWorkers_(0)
    {
        firstNumberToCalculate_ = tools::random(0, 100);
        secondNumberToCalculate_ = tools::random(0, 100);;
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
                Logger() << "Unsupported operation" << std::endl;
        }
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

    void assignWorker() { nrOfAssignedWorkers_++; }
    bool isEnoughWorkers() { return nrOfNeededWorkers_ == nrOfAssignedWorkers_; }
    Type getOperationType() { return type_; }
    unsigned getOperationNr() { return operNr_; }
    double getFirst() { return firstNumberToCalculate_; }
    double getSecond() { return secondNumberToCalculate_; }
private:
    Type type_;
    unsigned nrOfNeededWorkers_;
    const unsigned operNr_;
    std::atomic<unsigned> nrOfAssignedWorkers_;
    double firstNumberToCalculate_;
    double secondNumberToCalculate_;
};

using OperationPtr = std::shared_ptr<Operation>;

#endif // OPERATION_HPP
