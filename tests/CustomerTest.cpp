#include "gtest/gtest.h"

#include "Customer.hpp"
#include "mocks/ProgramStopControllerHelperMock.hpp"
#include "mocks/ShopMock.hpp"

namespace tests
{

using namespace testing;

class CustomerShould : public Test
{
public:
    CustomerShould() {}

    std::shared_ptr<mocks::ProgramStopControllerHelperMock> programStopControllerHelperMock_{
        std::make_shared<mocks::ProgramStopControllerHelperMock>()};
    std::shared_ptr<mocks::ShopMock> shopMock_{std::make_shared<mocks::ShopMock>()};
    Customer sut_{programStopControllerHelperMock_, shopMock_};
    const unsigned threadNr_{1};
};

TEST_F(CustomerShould, doNothingIfShopIsClosed)
{
    EXPECT_CALL(*programStopControllerHelperMock_, wasStopRequested())
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*shopMock_, isOpen()).WillOnce(Return(false));
    EXPECT_CALL(*shopMock_, buy(_)).Times(0);
    sut_.start(threadNr_);
}

TEST_F(CustomerShould, requestProductFromShop)
{
    EXPECT_CALL(*programStopControllerHelperMock_, wasStopRequested())
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*shopMock_, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*shopMock_, buy(_)).WillOnce(Return(std::make_shared<Product>(Product::Type::addition, 42)));
    sut_.start(threadNr_);
}

} // namespace tests
