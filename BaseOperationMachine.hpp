#ifndef BASEOPERATIONMACHINE_HPP
#define BASEOPERATIONMACHINE_HPP

class IBaseOperationMachine
{
public:
	virtual ~IBaseOperationMachine() {};

    virtual bool isBusy() = 0;
    virtual void calculate() = 0;

	bool isCalculationOngoing_{ false };
};

#endif // BASEOPERATIONMACHINE_HPP
