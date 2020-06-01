#include <thread>

#include "gtest/gtest.h"

#include "MachinesService.hpp"
#include "mocks/ProgramStopControllerHelperMock.hpp"

namespace tests
{

using namespace testing;

class MachineServiceShould : public testing::Test
{
public:
    MachineServiceShould() {}
    OperationPtr prepareOperation(Operation::Type type)
    {
        return std::make_shared<Operation>(type, operNr++);
    }
    std::shared_ptr<mocks::ProgramStopControllerHelperMock> programStopControllerHelperMock_{
        std::make_shared<mocks::ProgramStopControllerHelperMock>()};
    unsigned operNr{1};
    unsigned nrOfMachines{1};
    std::unique_ptr<MachinesService> sut_;
};

TEST_F(MachineServiceShould, registerAndPerformCalculationForAdditionOperation)
{
    ON_CALL(*programStopControllerHelperMock_, wasStopRequested()).WillByDefault(Return(false));
    sut_ = std::make_unique<MachinesService>(programStopControllerHelperMock_, nrOfMachines);

    auto oper = prepareOperation(Operation::Type::addition);
    auto operResult{
        std::make_shared<OperationResult>(oper->getOperationNr(), oper->getFirst() + oper->getSecond(), oper->getOperationType())};

    const auto promise = sut_->registerOperation(oper);

    ASSERT_TRUE(*promise.get() == *operResult);
    // End of work threads from machine pool
    EXPECT_CALL(*programStopControllerHelperMock_, wasStopRequested()).WillOnce(Return(true));
}

TEST_F(MachineServiceShould, registerAndPerformCalculationForMultiplicationOperation)
{
    ON_CALL(*programStopControllerHelperMock_, wasStopRequested()).WillByDefault(Return(false));
    sut_ = std::make_unique<MachinesService>(programStopControllerHelperMock_, nrOfMachines);

    auto oper = prepareOperation(Operation::Type::multiplication);
    auto operResult{
        std::make_shared<OperationResult>(oper->getOperationNr(), oper->getFirst() * oper->getSecond(), oper->getOperationType())};

    auto t = std::thread([this, &oper]()
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            Logger() << "Simulate async job to register other worker" << std::endl;
            sut_->registerOperation(oper);
        });

    const auto promise = sut_->registerOperation(oper);

    t.join();

    ASSERT_TRUE(*promise.get() == *operResult);
    // End of work threads from machine pool
    EXPECT_CALL(*programStopControllerHelperMock_, wasStopRequested()).WillRepeatedly(Return(true));
}

} // namespace tests
