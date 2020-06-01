#include <atomic>
#include <thread>

#include "gtest/gtest.h"

#include "Tools/Randoms.hpp"
#include "Tools/ThreadSafeQueue.hpp"

namespace tests
{

using namespace testing;

TEST(ThreadSafeQueueShould, supportBasicOperations)
{
    tools::ThreadSafeQueue<int> sut;

    int toPush = tools::random(1, 85214321);
    int result;
    ASSERT_TRUE(sut.empty());
    ASSERT_FALSE(sut.try_pop(result));

    sut.push(toPush);

    ASSERT_FALSE(sut.empty());
    ASSERT_TRUE(sut.size() == 1);
    ASSERT_TRUE(sut.try_pop(result));
    ASSERT_TRUE(sut.empty());
    ASSERT_TRUE(result = toPush);

    int toPush2 = tools::random(1, 85214321);
    sut.push(toPush2);
    sut.push(toPush);
    ASSERT_TRUE(sut.size() == 2);
    ASSERT_FALSE(sut.empty());

    sut.wait_and_pop(result);
    ASSERT_TRUE(sut.size() == 1);
    ASSERT_FALSE(sut.empty());
    ASSERT_TRUE(result = toPush2);
}

TEST(ThreadSafeQueueShould, waitIfQueueEmptyAndPopWhenElementPushed)
{
    tools::ThreadSafeQueue<int> sut;

    int toPush = tools::random(1, 85214321);
    int result;

    ASSERT_TRUE(sut.empty());

    auto t1 = std::thread([&sut, &result]()
        {
            sut.wait_and_pop(result);
        });
    auto t2 = std::thread([&sut, toPush]()
        {
            std::this_thread::sleep_for(std::chrono::seconds(tools::random(2,3)));
            sut.push(toPush);
        });

    t1.join();
    t2.join();

    ASSERT_TRUE(result = toPush);
    ASSERT_TRUE(sut.empty());
    ASSERT_TRUE(sut.size() == 0);
    ASSERT_FALSE(sut.try_pop(result));
}

TEST(ThreadSafeQueueShould, supportOperationsForMultipleThreads)
{
    tools::ThreadSafeQueue<int> sut;

    ASSERT_TRUE(sut.empty());
    std::atomic_uint nrOfFailedPop{0};
    auto t1 = std::thread([&sut]()
        {
            int toPush = tools::random(1, 85214321);
            std::this_thread::sleep_for(std::chrono::seconds(tools::random(1,3)));
            sut.push(toPush);
        });
    auto t2 = std::thread([&sut]()
        {
            int toPush = tools::random(1, 85214321);
            std::this_thread::sleep_for(std::chrono::seconds(tools::random(1,3)));
            sut.push(toPush);
        });
    auto t3 = std::thread([&sut, &nrOfFailedPop]()
        {
            int result;
            std::this_thread::sleep_for(std::chrono::seconds(tools::random(1,3)));
            if (!sut.try_pop(result)) nrOfFailedPop++;
        });
    auto t4 = std::thread([&sut]()
        {
            int toPush = tools::random(1, 85214321);
            std::this_thread::sleep_for(std::chrono::seconds(tools::random(1,3)));
            sut.push(toPush);
        });
    auto t5 = std::thread([&sut, &nrOfFailedPop]()
        {
            int result;
            std::this_thread::sleep_for(std::chrono::seconds(tools::random(1,3)));
            if (!sut.try_pop(result)) nrOfFailedPop++;
        });

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    ASSERT_FALSE(sut.empty());
    ASSERT_TRUE(sut.size() == 1+nrOfFailedPop);
}

} // namespace tests

