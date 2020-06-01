#include "gtest/gtest.h"

#include "Transport.hpp"
#include "mocks/ProgramStopControllerHelperMock.hpp"
#include "mocks/ShopMock.hpp"
#include "mocks/WarehouseEntryPointMock.hpp"

namespace tests
{

using namespace testing;

class TransportShould : public Test
{
public:
    TransportShould() {}

    std::shared_ptr<mocks::ProgramStopControllerHelperMock> programStopControllerHelperMock_{
        std::make_shared<mocks::ProgramStopControllerHelperMock>()};
    std::shared_ptr<mocks::ShopMock> shopMock_{std::make_shared<mocks::ShopMock>()};
    std::shared_ptr<mocks::WarehouseEntryPointMock> warehouseMock_{std::make_shared<mocks::WarehouseEntryPointMock>()};
    Transport sut_{programStopControllerHelperMock_, warehouseMock_, shopMock_};
    unsigned transporterNr_{2};
};

TEST_F(TransportShould, arriveToWarehouseAndTransportProductToShop)
{
    EXPECT_CALL(*programStopControllerHelperMock_, wasStopRequested())
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    auto product = std::make_shared<Product>(Product::Type::multiplication, 9998);
    EXPECT_CALL(*warehouseMock_, notifyAboutNewTransporter(transporterNr_)).WillOnce(Return(product));
    EXPECT_CALL(*shopMock_, deliver(product));
    sut_.start(transporterNr_);
}

TEST_F(TransportShould, noDeliverToShopIfUnableToGetProductFromWarehouse)
{
    EXPECT_CALL(*programStopControllerHelperMock_, wasStopRequested())
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*warehouseMock_, notifyAboutNewTransporter(transporterNr_)).WillOnce(Return(nullptr));
    EXPECT_CALL(*shopMock_, deliver(_)).Times(0);
    sut_.start(transporterNr_);
}

} // namespace tests

