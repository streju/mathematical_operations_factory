#ifndef BASEOPERATIONMACHINE_HPP
#define BASEOPERATIONMACHINE_HPP

class IBaseOperationMachine
{
public:
    virtual ~IBaseOperationMachine() {}

    bool isBusy()
    {
        return isCalculationOngoing_;
    }
    virtual void calculate() = 0;

	bool isCalculationOngoing_{ false };
};

#endif // BASEOPERATIONMACHINE_HPP
