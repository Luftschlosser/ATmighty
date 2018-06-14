/*!
 * This file implements the 16bit-Timer allocation- and freeing-methods of class \see AbstractHardwareManager
 */


#include "AbstractHardwareManager.h"


//specialization for allocation of any 16bit-timer
template<> AbstractTimer16bit* AbstractHardwareManager::allocTimer16bit<AbstractTimer16bit>()
{
	return allocItem<AbstractTimer1>();
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
template void AbstractHardwareManager::freeItem<AbstractTimer1>(AbstractTimer1 **hardware);
