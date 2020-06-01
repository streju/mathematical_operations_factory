#include <condition_variable>
#include <thread>

#include "gtest/gtest.h"

#include "Tools/Randoms.hpp"
#include "WorkersPool.hpp"
#include "mocks/ProgramStopControllerHelperMock.hpp"

namespace tests
{

using namespace testing;

ACTION_P(ReturnNegativePointee, pointer) { return !(*pointer); }

class WorkersPoolShould : public Test
{
public:
    WorkersPoolShould() {}

    std::shared_ptr<mocks::ProgramStopControllerHelperMock> programStopControllerHelperMock_{
        std::make_shared<mocks::ProgramStopControllerHelperMock>()};
    int nrOfThreads_{3};
    std::condition_variable cv_;
    std::mutex mu_;
    std::unique_ptr<WorkersPool> sut_;
};

TEST_F(WorkersPoolShould, executeSingleAdditionTask)
{
    ON_CALL(*programStopControllerHelperMock_, wasStopRequested()).WillByDefault(Return(false));
    WorkersPool sut{programStopControllerHelperMock_, nrOfThreads_};

    bool stopRequested{false};
    MockFunction<void(int)> funcToExecute;

    EXPECT_CALL(*programStopControllerHelperMock_, wasStopRequested())
        .WillRepeatedly(ReturnPointee(&stopRequested));
    EXPECT_CALL(funcToExecute, Call(_))
        .WillOnce(InvokeWithoutArgs([this, &stopRequested](){stopRequested=true; cv_.notify_all();}));
    sut.pushToQueue(funcToExecute.AsStdFunction(), WorkerHelper::WorkType::Calculation, Operation::Type::addition);

    std::unique_lock lk(mu_);
    cv_.wait(lk, [&stopRequested](){return stopRequested;});
}

TEST_F(WorkersPoolShould, executeSingleDivisionTask)
{
    ON_CALL(*programStopControllerHelperMock_, wasStopRequested()).WillByDefault(Return(false));
    WorkersPool sut{programStopControllerHelperMock_, nrOfThreads_};

    int nrOfActions{2};
    MockFunction<void(int)> funcToExecute;

    const auto funcToInvoke = [this, &nrOfActions]()
        {
            nrOfActions--;
            cv_.notify_all();
        };

    EXPECT_CALL(*programStopControllerHelperMock_, wasStopRequested())
        .WillRepeatedly(ReturnNegativePointee(&nrOfActions));
    EXPECT_CALL(funcToExecute, Call(_)).Times(2)
        .WillOnce(InvokeWithoutArgs(funcToInvoke))
        .WillOnce(InvokeWithoutArgs(funcToInvoke));
    sut.pushToQueue(funcToExecute.AsStdFunction(), WorkerHelper::WorkType::Calculation, Operation::Type::division);

    std::unique_lock lk(mu_);
    cv_.wait(lk, [&nrOfActions](){return !nrOfActions;});
}

TEST_F(WorkersPoolShould, executeSingleTransportTask)
{
    ON_CALL(*programStopControllerHelperMock_, wasStopRequested()).WillByDefault(Return(false));
    WorkersPool sut{programStopControllerHelperMock_, nrOfThreads_};

    bool stopRequested{false};
    MockFunction<void(int)> funcToExecute;

    EXPECT_CALL(*programStopControllerHelperMock_, wasStopRequested())
        .WillRepeatedly(ReturnPointee(&stopRequested));
    EXPECT_CALL(funcToExecute, Call(_))
        .WillOnce(InvokeWithoutArgs([this, &stopRequested](){stopRequested=true; cv_.notify_all();}));
    sut.pushToQueue(funcToExecute.AsStdFunction(), WorkerHelper::WorkType::Transport);

    std::unique_lock lk(mu_);
    cv_.wait(lk, [&stopRequested](){return stopRequested;});
}

TEST_F(WorkersPoolShould, executeMultipleTasks)
{
    ON_CALL(*programStopControllerHelperMock_, wasStopRequested()).WillByDefault(Return(false));
    WorkersPool sut{programStopControllerHelperMock_, nrOfThreads_};

    std::atomic_uint nrOfActions{4};
    MockFunction<void(int)> funcToExecute;
    MockFunction<void(int)> funcToExecute2;
    MockFunction<void(int)> funcToExecute3;

    const auto funcToInvoke = [this, &nrOfActions]()
        {
            nrOfActions--;
            cv_.notify_all();
        };

    EXPECT_CALL(*programStopControllerHelperMock_, wasStopRequested())
        .WillRepeatedly(ReturnNegativePointee(&nrOfActions));
    EXPECT_CALL(funcToExecute2, Call(_)).Times(2)
        .WillOnce(InvokeWithoutArgs(funcToInvoke))
        .WillOnce(InvokeWithoutArgs(funcToInvoke));
    EXPECT_CALL(funcToExecute, Call(_))
        .WillOnce(InvokeWithoutArgs(funcToInvoke));
    EXPECT_CALL(funcToExecute3, Call(_))
        .WillOnce(InvokeWithoutArgs(funcToInvoke));

    sut.pushToQueue(funcToExecute.AsStdFunction(), WorkerHelper::WorkType::Calculation, Operation::Type::substraction);
    sut.pushToQueue(funcToExecute2.AsStdFunction(), WorkerHelper::WorkType::Calculation, Operation::Type::division);
    sut.pushToQueue(funcToExecute3.AsStdFunction(), WorkerHelper::WorkType::Transport);

    std::unique_lock lk(mu_);
    cv_.wait(lk, [&nrOfActions](){return !nrOfActions;});
}

TEST_F(WorkersPoolShould, doNotingIfProgramWasStopped)
{
    MockFunction<void(int)> funcToExecute;

    ON_CALL(*programStopControllerHelperMock_, wasStopRequested()).WillByDefault(Return(true));
    WorkersPool sut{programStopControllerHelperMock_, nrOfThreads_};

    EXPECT_CALL(funcToExecute, Call(_)).Times(0);
    sut.pushToQueue(funcToExecute.AsStdFunction(), WorkerHelper::WorkType::Calculation, Operation::Type::multiplication);
}

} // namespace tests

