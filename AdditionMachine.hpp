#pragma once

#include "BaseOperationMachine.hpp"

class AdditionMachine :
	public IBaseOperationMachine
{
public:
	AdditionMachine();
	bool isBusy() override;
	void calculate() override;
};

