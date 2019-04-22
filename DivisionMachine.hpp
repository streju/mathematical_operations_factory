#pragma once

#include "BaseOperationMachine.hpp"

class DivisionMachine :
	public IBaseOperationMachine
{
public:
	DivisionMachine();
	void calculate() override;
};

