#include "gtest/gtest.h"

#include "Machines.hpp"

namespace tests
{

class MachineShould : public testing::Test
{
public:
    MachineShould() {}
    OperationPtr prepareOperation(Operation::Type type)
    {
        return std::make_shared<Operation>(type, operNr++);
    }
    Machine sut_;
    unsigned operNr{1};
    unsigned machineNr{1};
};

TEST_F(MachineShould, performCalculationForAdditionOperation)
{
    auto oper = prepareOperation(Operation::Type::addition);
    auto operResult{
        std::make_shared<OperationResult>(oper->getOperationNr(), oper->getFirst() + oper->getSecond(), oper->getOperationType())};
    ASSERT_TRUE(*sut_.performCalculation(oper, machineNr) == *operResult);
}

TEST_F(MachineShould, performCalculationForSubstractionOperation)
{
    auto oper = prepareOperation(Operation::Type::substraction);
    auto operResult{
        std::make_shared<OperationResult>(oper->getOperationNr(), oper->getFirst() - oper->getSecond(), oper->getOperationType())};
    ASSERT_TRUE(*sut_.performCalculation(oper, machineNr) == *operResult);
}

TEST_F(MachineShould, performCalculationForMultiplicationOperation)
{
    auto oper = prepareOperation(Operation::Type::multiplication);
    auto operResult{
        std::make_shared<OperationResult>(oper->getOperationNr(), oper->getFirst() * oper->getSecond(), oper->getOperationType())};
    ASSERT_TRUE(*sut_.performCalculation(oper, machineNr) == *operResult);
}

TEST_F(MachineShould, performCalculationForDivisionOperation)
{
    auto oper = prepareOperation(Operation::Type::division);
    auto operResult{
        std::make_shared<OperationResult>(oper->getOperationNr(), oper->getFirst() / oper->getSecond(), oper->getOperationType())};

    ASSERT_TRUE(*sut_.performCalculation(oper, machineNr) == *operResult);
}

} // namespace tests
