#include <condition_variable>
#include <thread>

#include "gtest/gtest.h"

#include "Tools/Randoms.hpp"
#include "Tools/ThreadsPool.hpp"
#include "mocks/ProgramStopControllerHelperMock.hpp"

namespace tests
{

using namespace testing;

ACTION_P(ReturnNegativePointee, pointer) { return !(*pointer); }

class ThreadsPoolShould : public Test
{
public:
    ThreadsPoolShould() {}

    std::shared_ptr<mocks::ProgramStopControllerHelperMock> programStopControllerHelperMock_{
        std::make_shared<mocks::ProgramStopControllerHelperMock>()};
    int nrOfThreads_{3};
    std::condition_variable cv_;
    std::mutex mu_;
};

TEST_F(ThreadsPoolShould, executeSingleTask)
{
    ON_CALL(*programStopControllerHelperMock_, wasStopRequested()).WillByDefault(Return(false));
    tools::ThreadPool sut{programStopControllerHelperMock_, nrOfThreads_};

    bool stopRequested{false};
    MockFunction<void(int)> funcToExecute;

    EXPECT_CALL(*programStopControllerHelperMock_, wasStopRequested())
        .WillRepeatedly(ReturnPointee(&stopRequested));
    EXPECT_CALL(funcToExecute, Call(_))
        .WillOnce(InvokeWithoutArgs([this, &stopRequested](){stopRequested=true; cv_.notify_one();}));
    sut.pushToQueue(funcToExecute.AsStdFunction());

    std::unique_lock lk(mu_);
    cv_.wait(lk, [&stopRequested](){return stopRequested;});
}

TEST_F(ThreadsPoolShould, executeMultipleTasks)
{
    ON_CALL(*programStopControllerHelperMock_, wasStopRequested()).WillByDefault(Return(false));
    tools::ThreadPool sut{programStopControllerHelperMock_, nrOfThreads_};

    MockFunction<void(int)> funcToExecute;
    MockFunction<void(int)> funcToExecute2;
    std::atomic_uint nrOfActions{3};

    const auto funcToInvoke = [this, &nrOfActions]()
        {
            nrOfActions--;
            cv_.notify_all();
        };

    EXPECT_CALL(funcToExecute, Call(_)).Times(2)
        .WillOnce(InvokeWithoutArgs(funcToInvoke))
        .WillOnce(InvokeWithoutArgs(funcToInvoke));
    EXPECT_CALL(funcToExecute2, Call(_))
        .WillOnce(InvokeWithoutArgs(funcToInvoke));

    EXPECT_CALL(*programStopControllerHelperMock_, wasStopRequested())
        .WillRepeatedly(ReturnNegativePointee(&nrOfActions));

    sut.pushToQueue(funcToExecute.AsStdFunction());
    sut.pushToQueue(funcToExecute2.AsStdFunction());
    sut.pushToQueue(funcToExecute.AsStdFunction());

    std::unique_lock lk(mu_);
    cv_.wait(lk, [&nrOfActions](){return !nrOfActions;});
}

TEST_F(ThreadsPoolShould, doNotingIfProgramWasStopped)
{
    MockFunction<void(int)> funcToExecute;

    ON_CALL(*programStopControllerHelperMock_, wasStopRequested()).WillByDefault(Return(true));
    tools::ThreadPool sut{programStopControllerHelperMock_, nrOfThreads_};

    EXPECT_CALL(funcToExecute, Call(_)).Times(0);
    sut.pushToQueue(funcToExecute.AsStdFunction());
}

} // namespace tests

