#pragma once

#include <gmock/gmock.h>

#include "Tools/IProgramStopControllerHelper.hpp"

namespace mocks
{

class ProgramStopControllerHelperMock : public tools::IProgramStopControllerHelper
{
public:
    MOCK_METHOD0(wasStopRequested, bool());
    MOCK_METHOD0(stop, void());
};

} // namespace mocks
