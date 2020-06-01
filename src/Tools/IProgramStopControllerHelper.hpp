#pragma once

#include <memory>

namespace tools
{

class IProgramStopControllerHelper
{
public:
    virtual bool wasStopRequested() = 0;
    virtual void stop() = 0;
    virtual ~IProgramStopControllerHelper() = default;
};

using ProgramStopControllerPtr = std::shared_ptr<tools::IProgramStopControllerHelper>;

} //namespace tools
