#pragma once

namespace tools
{

class IProgramStopControllerHelper
{
public:
    virtual bool wasStopRequested() = 0;
    virtual void stop() = 0;
    virtual ~IProgramStopControllerHelper() = default;
};

} //namespace tools
