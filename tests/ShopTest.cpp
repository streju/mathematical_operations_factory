#include "gtest/gtest.h"

#include "Shop.hpp"

namespace tests
{

using namespace testing;

class ShopShould : public Test
{
public:
    ShopShould() {}
    Shop sut_;
    unsigned customerNr_{31};
    ProductPtr prepareProduct(const Product::Type type = Product::Type::addition, double value = 42)
    {
        return std::make_shared<Product>(type, value);
    }
    CustomerRequest prepareCustomerRequestByProductType(Product::Type productType)
    {
        return CustomerRequest(customerNr_, CustomerRequest::Type::productType, productType);
    }

    CustomerRequest prepareCustomerRequestByValue(double minValue, double maxValue)
    {
        return CustomerRequest(customerNr_, CustomerRequest::Type::value, minValue, maxValue);
    }
    CustomerRequest prepareCustomerRequestByValueAndType(Product::Type productType, double minValue, double maxValue)
    {
        return CustomerRequest(customerNr_, CustomerRequest::Type::valueAndProductType, productType, minValue, maxValue);
    }
};

TEST_F(ShopShould, beClosedWhenThereIsNotAnyProduct)
{
    ASSERT_FALSE(sut_.isOpen());
}

TEST_F(ShopShould, beOpenWhenThereAreProducts)
{
    sut_.deliver(prepareProduct());
    ASSERT_TRUE(sut_.isOpen());
}

TEST_F(ShopShould, returnNothingIfAnyProductRequestedButNoProductsAvailable)
{
    ASSERT_TRUE(nullptr == sut_.buy(CustomerRequest(customerNr_)));
}

TEST_F(ShopShould, returnFirstDeliveredProductIfAnyProductIsRequested)
{
    auto product = prepareProduct(Product::Type::multiplication, 62);
    sut_.deliver(product);
    sut_.deliver(prepareProduct());

    ASSERT_TRUE(product == sut_.buy(CustomerRequest(customerNr_)));
}

TEST_F(ShopShould, returnMatchedProductByType)
{
    sut_.deliver(prepareProduct(Product::Type::multiplication, 62));
    sut_.deliver(prepareProduct());
    auto product = prepareProduct(Product::Type::division, 2);
    sut_.deliver(product);
    sut_.deliver(prepareProduct(Product::Type::substraction, 87));

    ASSERT_TRUE(product == sut_.buy(prepareCustomerRequestByProductType(Product::Type::division)));
    ASSERT_TRUE(nullptr == sut_.buy(prepareCustomerRequestByProductType(Product::Type::division)));
}

TEST_F(ShopShould, returnMatchedProductByValue)
{
    sut_.deliver(prepareProduct(Product::Type::multiplication, 62));
    sut_.deliver(prepareProduct());
    sut_.deliver(prepareProduct(Product::Type::division, 2));
    auto product = prepareProduct(Product::Type::substraction, 87);
    sut_.deliver(product);

    ASSERT_TRUE(product == sut_.buy(prepareCustomerRequestByValue(62.3, 87.6)));
    ASSERT_TRUE(nullptr == sut_.buy(prepareCustomerRequestByValue(62.3, 87.6)));
}

TEST_F(ShopShould, returnMatchedProductByValueAndType)
{
    auto product1 = prepareProduct(Product::Type::multiplication, 62);
    auto product2 = prepareProduct();
    auto product3 = prepareProduct(Product::Type::division, 2);
    auto product4 = prepareProduct(Product::Type::addition, 87);

    sut_.deliver(product1);
    sut_.deliver(product2);
    sut_.deliver(product3);
    sut_.deliver(product4);

    ASSERT_TRUE(product2 == sut_.buy(prepareCustomerRequestByValueAndType(Product::Type::addition, 40.3, 87.6)));
    ASSERT_TRUE(product4 == sut_.buy(prepareCustomerRequestByValueAndType(Product::Type::addition, 40.3, 87.6)));
    ASSERT_TRUE(sut_.isOpen());
    ASSERT_TRUE(nullptr == sut_.buy(prepareCustomerRequestByValueAndType(Product::Type::addition, 40.3, 87.6)));
}

TEST_F(ShopShould, closeShopWhenAllProductsBought)
{
    auto product1 = prepareProduct(Product::Type::multiplication, 62);
    auto product2 = prepareProduct();
    auto product3 = prepareProduct(Product::Type::division, 2);
    auto product4 = prepareProduct(Product::Type::substraction, 87);

    sut_.deliver(product1);
    sut_.deliver(product2);
    sut_.deliver(product3);
    sut_.deliver(product4);

    ASSERT_TRUE(sut_.isOpen());
    ASSERT_TRUE(product4 == sut_.buy(prepareCustomerRequestByValue(62.3, 87.6)));
    ASSERT_TRUE(product2 == sut_.buy(prepareCustomerRequestByProductType(Product::Type::addition)));
    ASSERT_TRUE(product1 == sut_.buy(CustomerRequest(customerNr_)));
    ASSERT_TRUE(product3 == sut_.buy(CustomerRequest(customerNr_)));
    ASSERT_FALSE(sut_.isOpen());
}

} // namespace tests

