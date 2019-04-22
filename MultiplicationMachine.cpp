#include "MultiplicationMachine.hpp"

MultiplicationMachine::MultiplicationMachine()
{
}


bool MultiplicationMachine::isBusy()
{
	return isCalculationOngoing_;
}
void MultiplicationMachine::calculate()
{
	isCalculationOngoing_ = true;

}