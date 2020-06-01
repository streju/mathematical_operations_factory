#include "gtest/gtest.h"

#include "Worker.hpp"
#include "mocks/MachinesServiceMock.hpp"
#include "mocks/WarehouseEntryPointMock.hpp"
#include "mocks/ProgramStopControllerHelperMock.hpp"

namespace tests
{

using namespace testing;

class WorkerShould : public Test
{
public:
    WorkerShould() {}

    std::shared_ptr<mocks::MachinesServiceMock> machinesServiceMock_{std::make_shared<mocks::MachinesServiceMock>()};
    std::shared_ptr<mocks::WarehouseEntryPointMock> warehouseMock_{std::make_shared<mocks::WarehouseEntryPointMock>()};
    std::shared_ptr<mocks::ProgramStopControllerHelperMock> programStopControllerHelperMock_{
        std::make_shared<mocks::ProgramStopControllerHelperMock>()};
    Worker sut_{machinesServiceMock_, warehouseMock_, programStopControllerHelperMock_};
    const unsigned threadNr_{1};
};

class WorkerWithParamShould
    : public WorkerShould
    , public WithParamInterface<Operation::Type>
{};

TEST_P(WorkerWithParamShould, processAndLoadOperation)
{
    const auto& operType = GetParam();
    std::promise<OperationResultPtr> promise;
    std::shared_future<OperationResultPtr> sh_fut{promise.get_future()};
    const auto operNr = 1;
    const auto operationPtr = std::make_shared<Operation>(operType, operNr);

    auto funcToExecutePromise = [&promise, &operType](){
        auto t = std::thread([operType](std::promise<OperationResultPtr>&& pr)
            {
                std::this_thread::sleep_for(std::chrono::seconds(tools::random(2,3)));
                Logger() << "Simulate async job to set promise" << std::endl;
                OperationResultPtr operRes = std::make_shared<OperationResult>(1,2, operType);
                pr.set_value(operRes);
            }, std::move(promise));
        t.detach();
    };

    EXPECT_CALL(*programStopControllerHelperMock_, wasStopRequested()).WillOnce(Return(false));
    EXPECT_CALL(*machinesServiceMock_, registerOperation(_)).WillOnce(DoAll(
        InvokeWithoutArgs(funcToExecutePromise),
        Return(sh_fut)));
    EXPECT_CALL(*warehouseMock_, placeOperResult(_));
    sut_.startProcessingOperation(threadNr_, operationPtr);

    EXPECT_CALL(*warehouseMock_, startLoadTransport(std::string("Worker nr" + std::to_string(threadNr_) + ": ")));
    sut_.loadTransport(threadNr_);
}

INSTANTIATE_TEST_CASE_P(ForDifferentOperationTypes,
    WorkerWithParamShould, ::testing::Values(
        Operation::Type::addition,
        Operation::Type::division,
        Operation::Type::multiplication,
        Operation::Type::substraction));

TEST_F(WorkerShould, loadOperationToTransporter)
{
    EXPECT_CALL(*warehouseMock_, startLoadTransport(std::string("Worker nr" + std::to_string(threadNr_) + ": ")));
    sut_.loadTransport(threadNr_);
}

TEST_F(WorkerShould, movePendings)
{
    EXPECT_CALL(*warehouseMock_, movePendings());
    sut_.movePendingsOperationsToWarehouse(threadNr_);
}

} // namespace tests
