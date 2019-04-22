#include "SubstractionMachine.hpp"

SubstractionMachine::SubstractionMachine()
{
}


bool SubstractionMachine::isBusy()
{
	return isCalculationOngoing_;
}
void SubstractionMachine::calculate()
{
	isCalculationOngoing_ = true;

}