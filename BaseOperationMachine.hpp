#ifndef BASEOPERATIONMACHINE_HPP
#define BASEOPERATIONMACHINE_HPP

class IBaseOperationMachine
{
    virtual ~IBaseOperationMachine();

    virtual isBusy() = 0;
    virtual calculate() = 0;
};

#endif // BASEOPERATIONMACHINE_HPP
