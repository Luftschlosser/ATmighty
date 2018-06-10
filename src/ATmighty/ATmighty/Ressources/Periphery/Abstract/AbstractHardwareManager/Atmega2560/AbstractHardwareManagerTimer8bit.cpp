/*!
 * This file implements the 8bit-Timer allocation- and freeing-methods of class \see AbstractHardwareManager
 */


#include "AbstractHardwareManager.h"


//specialization for allocation of any 8bit-timer
template<> AbstractTimer8bit* AbstractHardwareManager::allocTimer8bit<AbstractTimer8bit>()
{
	if (AbstractTimer0::Owner == 0)
	{
		return allocItem<AbstractTimer0>();
	}
	else
	{
		return allocItem<AbstractTimer2>();
	}
}

//specialization for allocation of any asynchronous 8bit-timer
template<> AbstractTimer8bitAsync* AbstractHardwareManager::allocTimer8bit<AbstractTimer8bitAsync>()
{
	return allocItem<AbstractTimer2>();
}

//unspecialized version for all 8bit-timers
template<class TimerHw> TimerHw* AbstractHardwareManager::allocTimer8bit()
{
	return allocItem<TimerHw>();
}

template<class Hw> void AbstractHardwareManager::freeItem(Hw **hardware)
{
	AbstractHardwareManager::Free<AbstractTimer8bit>((AbstractTimer8bit**) hardware, owner);
}


//The explizitely instantiated versions of the template-functions
template AbstractTimer0* AbstractHardwareManager::allocTimer8bit<AbstractTimer0>();
template AbstractTimer2* AbstractHardwareManager::allocTimer8bit<AbstractTimer2>();
template void AbstractHardwareManager::freeItem<AbstractTimer0>(AbstractTimer0 **hardware);
template void AbstractHardwareManager::freeItem<AbstractTimer2>(AbstractTimer2 **hardware);
template void AbstractHardwareManager::freeItem<AbstractTimer8bitAsync>(AbstractTimer8bitAsync **hardware);
