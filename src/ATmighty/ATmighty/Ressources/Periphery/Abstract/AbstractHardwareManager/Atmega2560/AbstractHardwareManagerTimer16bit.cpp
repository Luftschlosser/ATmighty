/*!
 * This file implements the 16bit-Timer allocation- and freeing-methods of class \see AbstractHardwareManager
 */


#include "AbstractHardwareManager.h"


//specialization for allocation of any 16bit-timer
template<> AbstractTimer16bit* AbstractHardwareManager::allocTimer16bit<AbstractTimer16bit>()
{
	if (AbstractTimer1::Owner == 0)
	{
		return allocItem<AbstractTimer1>();
	}
	else if (AbstractTimer3::Owner == 0)
	{
		return allocItem<AbstractTimer3>();
	}
	else if (AbstractTimer4::Owner == 0)
	{
		return allocItem<AbstractTimer4>();
	}
	else
	{
		return allocItem<AbstractTimer5>();
	}
}

//unspecialized version for all 16bit-timers
template<class TimerHw> TimerHw* AbstractHardwareManager::allocTimer16bit()
{
	return allocItem<TimerHw>();
}

template<class Hw> void AbstractHardwareManager::freeItem(Hw **hardware)
{
	AbstractHardwareManager::Free<AbstractTimer16bit>((AbstractTimer16bit**) hardware, owner);
}


//The explizitely instantiated versions of the template-functions
template AbstractTimer1* AbstractHardwareManager::allocTimer16bit<AbstractTimer1>();
template AbstractTimer3* AbstractHardwareManager::allocTimer16bit<AbstractTimer3>();
template AbstractTimer4* AbstractHardwareManager::allocTimer16bit<AbstractTimer4>();
template AbstractTimer5* AbstractHardwareManager::allocTimer16bit<AbstractTimer5>();
template void AbstractHardwareManager::freeItem<AbstractTimer1>(AbstractTimer1 **hardware);
template void AbstractHardwareManager::freeItem<AbstractTimer3>(AbstractTimer3 **hardware);
template void AbstractHardwareManager::freeItem<AbstractTimer4>(AbstractTimer4 **hardware);
template void AbstractHardwareManager::freeItem<AbstractTimer5>(AbstractTimer5 **hardware);
