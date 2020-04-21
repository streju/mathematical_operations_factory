#pragma once

#include <future>
#include "Tools/IProgramStopControllerHelper.hpp"

namespace tools
{

class ProgramStopControllerHelper : public IProgramStopControllerHelper
{
private:
    class ProgramStopController
    {
    public:
        static ProgramStopController& getInstance()
        {
            static ProgramStopController instance;
            return instance;
        }
        bool wasStopRequested(){return stopped_;}
        void stop(){stopped_ = true;}
    private:
        ProgramStopController()=default;
        ProgramStopController(ProgramStopController&&)=delete;
        ProgramStopController(const ProgramStopController&)=delete;
        ProgramStopController& operator=(ProgramStopController&&)=delete;
        ProgramStopController& operator=(const ProgramStopController&)=delete;
        std::atomic_bool stopped_{false};
    };
public:
    ProgramStopControllerHelper() = default;
    bool wasStopRequested() override {return ProgramStopController::getInstance().wasStopRequested();}
    void stop() override {ProgramStopController::getInstance().stop();}
};



} //namespace tools
