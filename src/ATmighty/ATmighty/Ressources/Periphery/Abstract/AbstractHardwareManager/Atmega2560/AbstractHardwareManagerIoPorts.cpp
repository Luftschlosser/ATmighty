/*!
 * This file implements the IoPort- allocation and freeing-methods of class \see AbstractHardwareManager
 */

#include "AbstractHardwareManager.h"
#include <util/atomic.h>


GeneralIoPort* AbstractHardwareManager::allocIoPort(char portChar)
{
	GeneralIoPort* instance = new GeneralIoPort(portChar);
	int8_t returnCode;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		returnCode = instance->init(owner);
	}

	if (returnCode == 0)
	{
		//Success
		#if ATMIGHTY_MESSAGELOG_ENABLE == true
		logAllocSuccess(instance->getHardwareStringRepresentation(), instance->getCharCode(), owner);
		#endif

		return instance;
	}
	else //physical hardware dependency could not be resolved
	{
		//"Already in use" is an impossible error, as the GeneralIoPort-instance is newly initialized and has no static owner-id.
		#if ATMIGHTY_MESSAGELOG_ENABLE == true
		logAllocFailDependency(instance->getHardwareStringRepresentation(), instance->getCharCode(), owner);
		#endif

		delete(instance);
		return nullptr;
	}
}

template<class Hw> void AbstractHardwareManager::freeItem(Hw **hardware)
{
	AbstractHardwareManager::Free<AbstractIoPort>((AbstractIoPort**) hardware, owner);
}


//The explizitely instantiated versions of the template-functions
template void AbstractHardwareManager::freeItem<AbstractIoPort>(AbstractIoPort **hardware);
template void AbstractHardwareManager::freeItem<GeneralIoPort>(GeneralIoPort **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPort<'A'>>(SpecificIoPort<'A'> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPort<'B'>>(SpecificIoPort<'B'> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPort<'C'>>(SpecificIoPort<'C'> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPort<'D'>>(SpecificIoPort<'D'> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPort<'E'>>(SpecificIoPort<'E'> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPort<'F'>>(SpecificIoPort<'F'> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPort<'G'>>(SpecificIoPort<'G'> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPort<'H'>>(SpecificIoPort<'H'> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPort<'J'>>(SpecificIoPort<'J'> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPort<'K'>>(SpecificIoPort<'K'> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPort<'L'>>(SpecificIoPort<'L'> **hardware);
