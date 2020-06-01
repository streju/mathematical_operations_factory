#include "gtest/gtest.h"

#include "Tools/Logger.hpp"
#include "Tools/ProgramStopController.hpp"
#include "Tools/Randoms.hpp"

namespace tests
{

TEST(ProgramStopControllerTest, shouldInformIfStopWasRequested)
{
    tools::ProgramStopControllerPtr sut{std::make_shared<tools::ProgramStopControllerHelper>()};

    ASSERT_FALSE(sut->wasStopRequested());

    auto threadToCallStop = std::thread([]()
        {
            std::this_thread::sleep_for(std::chrono::seconds(tools::random(1,3)));
            Logger() << "Simulate async job to call stop" << std::endl;
            tools::ProgramStopControllerPtr sut2{std::make_shared<tools::ProgramStopControllerHelper>()};

            sut2->stop();
        });
    threadToCallStop.join();

    ASSERT_TRUE(sut->wasStopRequested());
}

} // namespace tests
