#include "gtest/gtest.h"

#include "Manager.hpp"
#include "mocks/MachinesServiceMock.hpp"
#include "mocks/WarehouseEntryPointMock.hpp"
#include "mocks/WorkerMock.hpp"
#include "mocks/ProgramStopControllerHelperMock.hpp"

namespace tests
{

using namespace testing;

ACTION_P(ReturnNegativePointee, pointer) { return !(*pointer); }

class ManagerShould : public Test
{
public:
    ManagerShould(){}

    std::shared_ptr<mocks::ProgramStopControllerHelperMock> programStopControllerHelperMock_{
        std::make_shared<mocks::ProgramStopControllerHelperMock>()};
    std::shared_ptr<mocks::ProgramStopControllerHelperMock> programStopControllerHelperMockForWorkersPool_{
        std::make_shared<mocks::ProgramStopControllerHelperMock>()};
    std::shared_ptr<mocks::MachinesServiceMock> machinesServiceMock_{std::make_shared<mocks::MachinesServiceMock>()};
    std::shared_ptr<mocks::WarehouseEntryPointMock> warehouseMock_{std::make_shared<mocks::WarehouseEntryPointMock>()};
    std::shared_ptr<mocks::WorkerMock> workerMock_{std::make_shared<mocks::WorkerMock>()};
    unsigned maxTimeBetweenOperations_{10};
    unsigned minTimeBetweenOperations_{2};
    unsigned nrOfWorkers_{2};
    Manager sut_{programStopControllerHelperMock_, std::make_unique<WorkersPool>(programStopControllerHelperMockForWorkersPool_, nrOfWorkers_),
        machinesServiceMock_, warehouseMock_, workerMock_, minTimeBetweenOperations_, maxTimeBetweenOperations_};
};

TEST_F(ManagerShould, startOperationWhenPossibleAndLackOfTransport)
{
    EXPECT_CALL(*programStopControllerHelperMock_, wasStopRequested())
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*programStopControllerHelperMockForWorkersPool_, wasStopRequested())
        .WillOnce(Return(false)).WillOnce(Return(false))
        .WillOnce(Return(true)).WillOnce(Return(true));

    EXPECT_CALL(*warehouseMock_, getState()).WillOnce(
        Return(WarehouseRaport(false, false, false)));
    EXPECT_CALL(*workerMock_, startProcessingOperation(_, _)).Times(AtLeast(1));
    EXPECT_CALL(*workerMock_, loadTransport(_)).Times(0);
    sut_.start();
}

TEST_F(ManagerShould, loadTransportIfWarehouseIsFullButTransporterWaits)
{
    EXPECT_CALL(*programStopControllerHelperMock_, wasStopRequested())
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*programStopControllerHelperMockForWorkersPool_, wasStopRequested())
        .WillOnce(Return(false)).WillOnce(Return(false))
        .WillOnce(Return(true)).WillOnce(Return(true));

    EXPECT_CALL(*warehouseMock_, getState()).WillOnce(
        Return(WarehouseRaport(true, true, false)));
    EXPECT_CALL(*workerMock_, loadTransport(_));
    EXPECT_CALL(*workerMock_, startProcessingOperation(_, _)).Times(0);
    sut_.start();
}

TEST_F(ManagerShould, doNothingWhenWarehouseFullAndLackOfWaitingTransporters)
{
    EXPECT_CALL(*programStopControllerHelperMock_, wasStopRequested())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    EXPECT_CALL(*warehouseMock_, getState()).WillOnce(
        Return(WarehouseRaport(true, false, false)));
    EXPECT_CALL(*workerMock_, loadTransport(_)).Times(0);
    EXPECT_CALL(*workerMock_, startProcessingOperation(_, _)).Times(0);
    sut_.start();
}

TEST_F(ManagerShould, loadTransportAndStartOperationIfBothActionsArePossible)
{
    std::atomic_uint nrOfEndedActions{2};
    OperationPtr oper;
    EXPECT_CALL(*programStopControllerHelperMock_, wasStopRequested())
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*programStopControllerHelperMockForWorkersPool_, wasStopRequested())
        .WillRepeatedly(ReturnNegativePointee(&nrOfEndedActions));

    EXPECT_CALL(*warehouseMock_, getState()).WillOnce(
        Return(WarehouseRaport(false, true, false)));
    EXPECT_CALL(*workerMock_, loadTransport(_)).WillOnce(InvokeWithoutArgs([&nrOfEndedActions]{
        nrOfEndedActions--;}));
    EXPECT_CALL(*workerMock_, startProcessingOperation(_, _)).Times(AtLeast(1))
         .WillOnce(DoAll(
             SaveArg<1>(&oper),
             InvokeWithoutArgs([&oper, &nrOfEndedActions]{
                 if ((oper->getOperationType() == Operation::Type::multiplication
                     || oper->getOperationType() == Operation::Type::division))
                 {
                     nrOfEndedActions++;
                 }
                nrOfEndedActions--;
         })))
        .WillOnce(InvokeWithoutArgs([&nrOfEndedActions]{nrOfEndedActions--;}));
    sut_.start();
}

TEST_F(ManagerShould, doNothingIfProgramWasStopped)
{
    EXPECT_CALL(*programStopControllerHelperMock_, wasStopRequested()).WillRepeatedly(Return(true));

    EXPECT_CALL(*warehouseMock_, getState()).Times(0);
    EXPECT_CALL(*workerMock_, startProcessingOperation(_, _)).Times(0);
    EXPECT_CALL(*workerMock_, loadTransport(_)).Times(0);
    sut_.start();
}

} // namespace tests

