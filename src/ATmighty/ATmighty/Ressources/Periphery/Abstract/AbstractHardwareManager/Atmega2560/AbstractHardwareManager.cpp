/*!
 * This file implements the main general part of class \see AbstractHardwareManager
 * IoPin-Management is implemented in AbstractHardwareManagerIoPins.cpp, as IoPins need a different implementation of "free()" than the other abstract hardware-types.
 * IoPort-Management is implemented in AbstractHardwareManagerIoPorts.cpp.
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

//General free-function to be called from freeItem() to save memory from all the different template-instantiations
template<class ToFree> void AbstractHardwareManager::Free(ToFree **hardware, int8_t owner)
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

//General allocator: - Ports
template SpecificIoPort<'A'>* AbstractHardwareManager::allocItem<SpecificIoPort<'A'>>();
template SpecificIoPort<'B'>* AbstractHardwareManager::allocItem<SpecificIoPort<'B'>>();
template SpecificIoPort<'C'>* AbstractHardwareManager::allocItem<SpecificIoPort<'C'>>();
template SpecificIoPort<'D'>* AbstractHardwareManager::allocItem<SpecificIoPort<'D'>>();
template SpecificIoPort<'E'>* AbstractHardwareManager::allocItem<SpecificIoPort<'E'>>();
template SpecificIoPort<'F'>* AbstractHardwareManager::allocItem<SpecificIoPort<'F'>>();
template SpecificIoPort<'G'>* AbstractHardwareManager::allocItem<SpecificIoPort<'G'>>();
template SpecificIoPort<'H'>* AbstractHardwareManager::allocItem<SpecificIoPort<'H'>>();
template SpecificIoPort<'J'>* AbstractHardwareManager::allocItem<SpecificIoPort<'J'>>();
template SpecificIoPort<'K'>* AbstractHardwareManager::allocItem<SpecificIoPort<'K'>>();
template SpecificIoPort<'L'>* AbstractHardwareManager::allocItem<SpecificIoPort<'L'>>();

//General freeing:
template void AbstractHardwareManager::Free<AbstractIoPort>(AbstractIoPort** hardware, int8_t owner);
