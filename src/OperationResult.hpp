#pragma once

#include <memory>

#include "Operation.hpp"

struct OperationResult
{
    OperationResult(const OperationResult&) = delete;
    OperationResult* operator=(const OperationResult&) = delete;
    OperationResult(const unsigned operNr, double result, Operation::Type type)
        : operationNr_(operNr), value_(result), type_(type)
    {}
    const unsigned operationNr_;
    const double value_;
    const Operation::Type type_;
    bool operator==(const OperationResult& other)
    {
        return operationNr_ == other.operationNr_
            && value_ == other.value_
            && type_ == other.type_;
    }
};

using OperationResultPtr = std::shared_ptr<OperationResult>;
