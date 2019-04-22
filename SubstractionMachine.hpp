#pragma once

#include "BaseOperationMachine.hpp"

class SubstractionMachine :
	public IBaseOperationMachine
{
public:
	SubstractionMachine();
	bool isBusy() override;
	void calculate() override;
};

