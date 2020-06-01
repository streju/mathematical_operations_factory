#include "gtest/gtest.h"

#include "WarehouseEntryPoint.hpp"
#include "mocks/WarehouseMock.hpp"

namespace tests
{

using namespace testing;

MATCHER_P(WarehouseRaportEq, raport, "")
{
    return raport.isDepotFull == arg.isDepotFull
        && raport.isAnyTransporterWaiting == arg.isAnyTransporterWaiting
        && raport.isAnyOperationPending == arg.isAnyOperationPending;
}

class WarehouseEntryPointShould : public Test
{
public:
    WarehouseEntryPointShould() {}

    std::shared_ptr<mocks::WarehouseMock> warehouseMock_{std::make_shared<mocks::WarehouseMock>()};
    WarehouseEntryPoint sut_{warehouseMock_};
};

TEST_F(WarehouseEntryPointShould, putAditionAndSubstractionResultsToWarehouse)
{
    const auto substrationResult = std::make_shared<OperationResult>(2, 3, Operation::Type::substraction);
    EXPECT_CALL(*warehouseMock_, isFull()).WillOnce(Return(false));
    EXPECT_CALL(*warehouseMock_, put(substrationResult));
    sut_.placeOperResult(substrationResult);

    const auto additionResult = std::make_shared<OperationResult>(23, 97, Operation::Type::addition);
    EXPECT_CALL(*warehouseMock_, isFull()).WillOnce(Return(false));
    EXPECT_CALL(*warehouseMock_, put(additionResult));
    sut_.placeOperResult(additionResult);
}

TEST_F(WarehouseEntryPointShould, putMultiplicationAndDvivisonResultsToWarehouse)
{
    const auto divisionResult = std::make_shared<OperationResult>(1, 54.32, Operation::Type::division);
    sut_.placeOperResult(divisionResult);

    const auto multpilicationResult = std::make_shared<OperationResult>(4, 8965.143, Operation::Type::multiplication);
    sut_.placeOperResult(multpilicationResult);

    EXPECT_CALL(*warehouseMock_, isFull()).WillOnce(Return(false));
    EXPECT_CALL(*warehouseMock_, put(multpilicationResult));
    sut_.placeOperResult(multpilicationResult);

    EXPECT_CALL(*warehouseMock_, isFull()).WillOnce(Return(false));
    EXPECT_CALL(*warehouseMock_, put(divisionResult));
    sut_.placeOperResult(divisionResult);
}

TEST_F(WarehouseEntryPointShould, notifyAboutNewTransporter)
{
    unsigned transporterNr{2};
    EXPECT_CALL(*warehouseMock_, handleTransporterArrival(transporterNr));
    sut_.notifyAboutNewTransporter(transporterNr);
}

TEST_F(WarehouseEntryPointShould, startLoadTransport)
{
    std::string prefix{"Worker3000"};
    EXPECT_CALL(*warehouseMock_, startLoadTransport(prefix));
    sut_.startLoadTransport(prefix);
}

TEST_F(WarehouseEntryPointShould, provideInfoAboutWarehouseCurrentState)
{
    EXPECT_CALL(*warehouseMock_, isFull()).WillOnce(Return(false));
    EXPECT_CALL(*warehouseMock_, isAnyTransporterWaiting()).WillOnce(Return(false));
    ASSERT_THAT(sut_.getState(), WarehouseRaportEq(WarehouseRaport(false, false, false)));

    EXPECT_CALL(*warehouseMock_, isFull()).WillOnce(Return(true));
    EXPECT_CALL(*warehouseMock_, isAnyTransporterWaiting()).WillOnce(Return(true));
    ASSERT_THAT(sut_.getState(), WarehouseRaportEq(WarehouseRaport(true, true, false)));
}

TEST_F(WarehouseEntryPointShould, putOperResultToPendingsIfWarehouseFull)
{
    const auto substrationResult = std::make_shared<OperationResult>(2, 3, Operation::Type::substraction);
    EXPECT_CALL(*warehouseMock_, isFull()).WillOnce(Return(false));
    EXPECT_CALL(*warehouseMock_, put(substrationResult));
    sut_.placeOperResult(substrationResult);

    const auto divisionResult = std::make_shared<OperationResult>(1, 54.32, Operation::Type::division);
    sut_.placeOperResult(divisionResult);

    const auto multpilicationResult = std::make_shared<OperationResult>(4, 8965.143, Operation::Type::multiplication);
    sut_.placeOperResult(multpilicationResult);

    const auto additionResult = std::make_shared<OperationResult>(23, 97, Operation::Type::addition);
    EXPECT_CALL(*warehouseMock_, isFull()).WillOnce(Return(true));
    EXPECT_CALL(*warehouseMock_, put(additionResult)).Times(0);
    sut_.placeOperResult(additionResult);

    EXPECT_CALL(*warehouseMock_, isFull()).WillOnce(Return(true));
    EXPECT_CALL(*warehouseMock_, put(multpilicationResult)).Times(0);
    sut_.placeOperResult(multpilicationResult);

    EXPECT_CALL(*warehouseMock_, isFull()).WillOnce(Return(true));
    EXPECT_CALL(*warehouseMock_, put(divisionResult)).Times(0);
    sut_.placeOperResult(divisionResult);
}

TEST_F(WarehouseEntryPointShould, movePendingsIfWarehouseNotFull)
{
    const auto additionResult = std::make_shared<OperationResult>(23, 97, Operation::Type::addition);
    EXPECT_CALL(*warehouseMock_, isFull()).WillOnce(Return(true));
    EXPECT_CALL(*warehouseMock_, put(additionResult)).Times(0);
    sut_.placeOperResult(additionResult);

    const auto divisionResult = std::make_shared<OperationResult>(1, 54.32, Operation::Type::division);
    sut_.placeOperResult(divisionResult);

    const auto multpilicationResult = std::make_shared<OperationResult>(4, 8965.143, Operation::Type::multiplication);
    sut_.placeOperResult(multpilicationResult);

    EXPECT_CALL(*warehouseMock_, isFull()).WillOnce(Return(true));
    EXPECT_CALL(*warehouseMock_, put(multpilicationResult)).Times(0);
    sut_.placeOperResult(multpilicationResult);

    EXPECT_CALL(*warehouseMock_, isFull()).WillOnce(Return(true));
    EXPECT_CALL(*warehouseMock_, put(divisionResult)).Times(0);
    sut_.placeOperResult(divisionResult);

    EXPECT_CALL(*warehouseMock_, isFull()).WillRepeatedly(Return(true));
    EXPECT_CALL(*warehouseMock_, put(testing::_)).Times(0);
    sut_.movePendings();

    EXPECT_CALL(*warehouseMock_, isFull()).WillOnce(Return(false));
    EXPECT_CALL(*warehouseMock_, isAnyTransporterWaiting()).WillOnce(Return(true));
    ASSERT_THAT(sut_.getState(), WarehouseRaportEq(WarehouseRaport(false, true, true)));

    EXPECT_CALL(*warehouseMock_, isFull()).WillRepeatedly(Return(false));
    EXPECT_CALL(*warehouseMock_, put(additionResult));
    EXPECT_CALL(*warehouseMock_, put(multpilicationResult));
    EXPECT_CALL(*warehouseMock_, put(divisionResult));
    sut_.movePendings();
}

} // namespace tests

