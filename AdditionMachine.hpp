#pragma once

#include "BaseOperationMachine.hpp"

class AdditionMachine :
	public IBaseOperationMachine
{
public:
	AdditionMachine();
	void calculate() override;
};

