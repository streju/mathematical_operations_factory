#include "AdditionMachine.hpp"

AdditionMachine::AdditionMachine()
{
}


bool AdditionMachine::isBusy()
{
	return isCalculationOngoing_;
}
void AdditionMachine::calculate()
{
	isCalculationOngoing_ = true;

}