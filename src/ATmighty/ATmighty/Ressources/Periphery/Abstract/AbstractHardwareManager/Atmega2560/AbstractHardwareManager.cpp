/*!
 * This file implements the main part of class \see AbstractHardwareManager
 * IoPin-Management is implemented in AbstractHardwareManagerIoPins.cpp, as IoPins need a different implementation of "free()" than the other abstract hardware-types.
 * Static Message-logging helper-functions are implemented in "AbstractHardwareManagerLogging.cpp" to not waste too much space here.
 */

#include "AbstractHardwareManager.h"
#include <util/atomic.h>


AbstractHardwareManager::AbstractHardwareManager(int8_t ownerId) : owner(ownerId)
{
	if (owner == 0) //validate ownerID
	{
		owner = -1;
	}
}

template<class Hw> Hw* AbstractHardwareManager::allocItem ()
{
	Hw* instance = new Hw();
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
	else //Error
	{
		#if ATMIGHTY_MESSAGELOG_ENABLE == true
		if (returnCode > 0) //abstract hardware is already in use
		{
			logAllocFailUsed(instance->getHardwareStringRepresentation(), instance->getCharCode(), owner, instance->getOwner());
		}
		else //physical hardware dependency could not be resolved
		{
			logAllocFailDependency(instance->getHardwareStringRepresentation(), instance->getCharCode(), owner);
		}
		#endif

		delete(instance);
		return nullptr;
	}
}

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
	if (hardware != nullptr && (*hardware) != nullptr)
	{
		(*hardware)->exit();

		//Success-Message
		#if ATMIGHTY_MESSAGELOG_ENABLE == true
		logFreeSuccess((*hardware)->getHardwareStringRepresentation(), (*hardware)->getCharCode(), owner);
		#endif

		delete(*hardware);
		(*hardware)=nullptr;
	}
	#if ATMIGHTY_MESSAGELOG_ENABLE == true
	else
	{
		//Fail-Message
		logFreeFail((*hardware)->getHardwareStringRepresentation(), owner);
	}
	#endif
}


//The explizitely instantiated versions of the template-functions (update the list when adding new types!)

//Ports
template SpecificIoPort<'A'>* AbstractHardwareManager::allocItem<SpecificIoPort<'A'>>();
template SpecificIoPort<'B'>* AbstractHardwareManager::allocItem<SpecificIoPort<'B'>>();
template SpecificIoPort<'C'>* AbstractHardwareManager::allocItem<SpecificIoPort<'C'>>();
template SpecificIoPort<'D'>* AbstractHardwareManager::allocItem<SpecificIoPort<'D'>>();
template SpecificIoPort<'E'>* AbstractHardwareManager::allocItem<SpecificIoPort<'E'>>();
template SpecificIoPort<'F'>* AbstractHardwareManager::allocItem<SpecificIoPort<'F'>>();
template void AbstractHardwareManager::freeItem<AbstractIoPort>(AbstractIoPort **hardware);
template void AbstractHardwareManager::freeItem<GeneralIoPort>(GeneralIoPort **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPort<'A'>>(SpecificIoPort<'A'> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPort<'B'>>(SpecificIoPort<'B'> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPort<'C'>>(SpecificIoPort<'C'> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPort<'D'>>(SpecificIoPort<'D'> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPort<'E'>>(SpecificIoPort<'E'> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPort<'F'>>(SpecificIoPort<'F'> **hardware);
