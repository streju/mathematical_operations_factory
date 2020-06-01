#include <thread>

#include "gtest/gtest.h"

#include "Warehouse.hpp"
#include "mocks/ProgramStopControllerHelperMock.hpp"

namespace tests
{

using namespace testing;

class WarehouseShould : public Test
{
public:
    WarehouseShould() {}

    std::shared_ptr<mocks::ProgramStopControllerHelperMock> programStopControllerHelperMock_{
        std::make_shared<mocks::ProgramStopControllerHelperMock>()};
    std::unique_ptr<Warehouse> sut_;
};

TEST_F(WarehouseShould, putOperResultToDepotAndInformAboutDepotState)
{
    unsigned maxDepotSize{2};
    sut_ = std::make_unique<Warehouse>(maxDepotSize, programStopControllerHelperMock_);

    const auto additionResult = std::make_shared<OperationResult>(23, 97, Operation::Type::addition);
    sut_->put(additionResult);

    ASSERT_FALSE(sut_->isFull());

    const auto multpilicationResult = std::make_shared<OperationResult>(4, 8965.143, Operation::Type::multiplication);
    sut_->put(multpilicationResult);

    ASSERT_TRUE(sut_->isFull());

    const auto divisionResult = std::make_shared<OperationResult>(1, 54.32, Operation::Type::division);

    ASSERT_DEATH(sut_->put(divisionResult), "");
}

TEST_F(WarehouseShould, handleArrivalAndLoadTransporter)
{
    unsigned maxDepotSize{2};
    sut_ = std::make_unique<Warehouse>(maxDepotSize, programStopControllerHelperMock_);
    ProductPtr product;

    const auto multpilicationResult = std::make_shared<OperationResult>(4, 8965.143, Operation::Type::multiplication);
    sut_->put(multpilicationResult);

    auto funcToStartLoadingTransport = [this](){
        auto t = std::thread([this]()
            {
                std::this_thread::sleep_for(std::chrono::seconds(tools::random(2,3)));
                Logger() << "Simulate async job to start loading transport" << std::endl;

                ASSERT_TRUE(sut_->isAnyTransporterWaiting());
                std::string prefix{"Worker3000"};
                sut_->startLoadTransport(prefix);
            });
        t.detach();
    };

    unsigned transporterNr{2};
    EXPECT_CALL(*programStopControllerHelperMock_, wasStopRequested())
        .WillOnce(DoAll(
            InvokeWithoutArgs(funcToStartLoadingTransport), Return(false)))
        .WillRepeatedly(Return(false));
    product = sut_->handleTransporterArrival(transporterNr);
}

} // namespace tests

