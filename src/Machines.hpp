#pragma once

#include <memory>
#include <thread>

#include "Operation.hpp"
#include "OperationResult.hpp"
#include "Tools/Logger.hpp"
#include "Tools/Randoms.hpp"
#include "Tools/Timer.hpp"

class Machine
{
public:
    Machine() = default;

    OperationResultPtr performCalculation(const OperationPtr& operation, unsigned nr)
    {
        std::string prefix{"Machine nr" + std::to_string(nr) + ": "};
        const auto& operType = operation->getOperationType();
        const auto& firstNr = operation->getFirst();
        const auto& secondNr = operation->getSecond();

        printCalculation(operType, firstNr, secondNr, prefix);

        tools::SecondsTimer timer;
        const auto value = calculate(operType, firstNr, secondNr, prefix);

        Logger(prefix) << "Calculation result is " << value
            << ". Calculation Time: " << timer.stopAndGetTime() << "s." << std::endl;

        return std::make_shared<OperationResult>(operation->getOperationNr(), value, operType);
    }

private:
    double calculate(Operation::Type operType, double first, double second, const std::string& prefix)
    {
        switch (operType)
        {
            case Operation::Type::addition:
                std::this_thread::sleep_for(std::chrono::seconds(tools::random(2, 3)));
                return first + second;
            case Operation::Type::substraction:
                std::this_thread::sleep_for(std::chrono::seconds(tools::random(2, 4)));
                return first - second;
            case Operation::Type::multiplication:
                std::this_thread::sleep_for(std::chrono::seconds(tools::random(3, 5)));
                return first * second;
            case Operation::Type::division:
                std::this_thread::sleep_for(std::chrono::seconds(tools::random(4, 6)));
                return first / second;
        }
        Logger(prefix) << "Unsupported operation!" << std::endl;
        std::abort();
        return 0;
    }

    void printCalculation(const Operation::Type operType, double first, double second, const std::string& prefix)
    {
        std::string str{"Going to "};
        switch (operType)
        {
            case Operation::Type::addition:
                str += "add " + std::to_string(first) + " with " + std::to_string(second);
                break;
            case Operation::Type::substraction:
                str += "substract " + std::to_string(second) + " from " + std::to_string(first);;
                break;
            case Operation::Type::multiplication:
                str += "multiply " + std::to_string(first) + " with " + std::to_string(second);
                break;
            case Operation::Type::division:
                str += "divide " + std::to_string(first) + " by " + std::to_string(second);
                break;
            default:
                Logger(prefix) << "Unsuported operation" << std::endl;
                return;
        }
        Logger(prefix) << str << std::endl;
    }
};
