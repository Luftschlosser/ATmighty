/*!
 * This file implements the 8bit-Timer allocation- and freeing-methods of class \see AbstractHardwareManager
 */


#include "AbstractHardwareManager.h"


//specialization for allocation of any 8bit-timer
template<> AbstractTimer8bit* AbstractHardwareManager::allocTimer8bit<AbstractTimer8bit>()
{
	return allocItem<AbstractTimer0>();
}

template<class Hw> void AbstractHardwareManager::freeItem(Hw **hardware)
{
	AbstractHardwareManager::Free<AbstractTimer8bit>((AbstractTimer8bit**) hardware, owner);
}


//The explizitely instantiated versions of the template-functions
template void AbstractHardwareManager::freeItem<AbstractTimer0>(AbstractTimer0 **hardware);
