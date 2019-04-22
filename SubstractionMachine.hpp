#pragma once

#include "BaseOperationMachine.hpp"

class SubstractionMachine :
	public IBaseOperationMachine
{
public:
	SubstractionMachine();
	void calculate() override;
};

