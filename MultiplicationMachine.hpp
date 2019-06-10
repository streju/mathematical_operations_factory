#pragma once

#include "BaseOperationMachine.hpp"

class MultiplicationMachine :
	public IBaseOperationMachine
{
public:
	MultiplicationMachine();
	void calculate() override;
};

