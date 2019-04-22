#include "DivisionMachine.hpp"

DivisionMachine::DivisionMachine()
{
}


bool DivisionMachine::isBusy()
{
	return isCalculationOngoing_;
}
void DivisionMachine::calculate()
{
	isCalculationOngoing_ = true;

}