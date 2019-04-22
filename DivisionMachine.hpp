#pragma once

#include "BaseOperationMachine.hpp"

class DivisionMachine :
	public IBaseOperationMachine
{
public:
	DivisionMachine();
	bool isBusy() override;
	void calculate() override;
};

