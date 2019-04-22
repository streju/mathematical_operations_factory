#pragma once

#include "BaseOperationMachine.hpp"

class MultiplicationMachine :
	public IBaseOperationMachine
{
public:
	MultiplicationMachine();
	bool isBusy() override;
	void calculate() override;
};

