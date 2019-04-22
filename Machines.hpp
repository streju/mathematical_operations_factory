#pragma once

#include "BaseOperationMachine.hpp"
#include "Operation.hpp"

template<>
class AdditionMachine<Operation::Type::addition> :
	public IBaseOperationMachine
{
public:
	AdditionMachine();
	void calculate() override;
};

