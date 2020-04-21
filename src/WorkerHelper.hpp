#pragma once

#include <memory>
#include <string>
#include <utility>
#include <optional>

#include "IWorker.hpp"
#include "Operation.hpp"
#include "OperationResult.hpp"
#include "IMachinesService.hpp"
#include "IWarehouse.hpp"
#include "Tools/ThreadsPool.hpp"
#include "Tools/IProgramStopControllerHelper.hpp"

struct WorkerHelper
{
    enum class WorkType
    {
        Transport,
        Calculation
    };

    WorkerHelper() = default;
    WorkerHelper(const WorkerHelper& wh)
    {
        callback_ = wh.callback_;
        type = wh.type;
        operType = wh.operType;
        nrOfNeededWorkers = wh.nrOfNeededWorkers.load();
        nrOfAssignedWorkers = wh.nrOfAssignedWorkers.load();
    }
    WorkerHelper& operator=(const WorkerHelper& wh)
    {
        callback_ = wh.callback_;
        type = wh.type;
        operType = wh.operType;
        nrOfNeededWorkers = wh.nrOfNeededWorkers.load();
        nrOfAssignedWorkers = wh.nrOfAssignedWorkers.load();
        return *this;
    }

    WorkerHelper(WorkerHelper&& wh)
    {
        callback_ = wh.callback_;
        type = wh.type;
        operType = wh.operType;
        nrOfNeededWorkers = wh.nrOfNeededWorkers.load();
        nrOfAssignedWorkers = wh.nrOfAssignedWorkers.load();
    }

    WorkerHelper& operator=(WorkerHelper&& wh)
    {
        callback_ = wh.callback_;
        type = wh.type;
        operType = wh.operType;
        nrOfNeededWorkers = wh.nrOfNeededWorkers.load();
        nrOfAssignedWorkers = wh.nrOfAssignedWorkers.load();
        return *this;
    }

    void operator ()(int threadNr)
    {
        callback_(threadNr);
    }

    bool assignAndCheck()
    {
        ++nrOfAssignedWorkers;
        return nrOfAssignedWorkers == nrOfNeededWorkers;
    }

    void checkNrOfNeededWorkers(std::optional<Operation::Type> operType)
    {
        if (!operType && type == WorkType::Transport) {nrOfNeededWorkers = 1; return;}
        switch (*operType)
        {
            case Operation::Type::division:
            case Operation::Type::multiplication:
                nrOfNeededWorkers = 2;
                return;
            case Operation::Type::addition:
            case Operation::Type::substraction:
                nrOfNeededWorkers = 1;
                return;
            default:
                Logger() << "Unsupported operation" << std::endl;
        }
    }

    WorkerHelper(const std::function<void(const unsigned)>& f, WorkType type, Operation::Type op)
        : callback_(f), type(type), operType(op) {checkNrOfNeededWorkers(operType);}
    WorkerHelper(const std::function<void(const unsigned)>& f, WorkType type) : callback_(f), type(type)
        {checkNrOfNeededWorkers(std::nullopt);}

    std::function<void(const unsigned)> callback_;
    WorkType type;
    std::optional<Operation::Type> operType;
    std::atomic_uint nrOfNeededWorkers;
    std::atomic_uint nrOfAssignedWorkers{0};
};

