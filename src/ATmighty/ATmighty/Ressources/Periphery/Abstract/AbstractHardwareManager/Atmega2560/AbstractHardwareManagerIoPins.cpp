/*!
 * This file implements the IoPin- allocation and freeing-methods of class \see AbstractHardwareManager
 */

#include "AbstractHardwareManager.h"
#include <util/atomic.h>


GeneralIoPin* AbstractHardwareManager::allocIoPin(char portChar, uint8_t pinNumber)
{
	GeneralIoPin* instance = new GeneralIoPin(portChar, pinNumber);
	int8_t returnCode;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		returnCode = instance->init(owner);
	}

	if (returnCode == 0)
	{
		//Success
		#if ATMIGHTY_MESSAGELOG_ENABLE == true
		logAllocSuccess(instance->getHardwareStringRepresentation(), instance->getPinPort(), instance->getPinNumber(), owner);
		#endif

		return instance;
	}
	else //physical hardware dependency could not be resolved
	{
		//"Already in use" is an impossible error, as the GeneralIoPort-instance is newly initialized and has no static owner-id.
		#if ATMIGHTY_MESSAGELOG_ENABLE == true
		logAllocFailDependency(instance->getHardwareStringRepresentation(), instance->getPinPort(), instance->getPinNumber(), owner);
		#endif

		delete(instance);
		return nullptr;
	}
}

template<char PortChar, uint8_t PinNumber> SpecificIoPin<PortChar, PinNumber>* AbstractHardwareManager::allocIoPin()
{
	SpecificIoPin<PortChar, PinNumber>* instance = new SpecificIoPin<PortChar, PinNumber>();
	int8_t returnCode;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		returnCode = instance->init(owner);
	}

	if (returnCode == 0)
	{
		//Success
		#if ATMIGHTY_MESSAGELOG_ENABLE == true
		logAllocSuccess(instance->getHardwareStringRepresentation(), instance->getPinPort(), instance->getPinNumber(), owner);
		#endif

		return instance;
	}
	else //Error
	{
		#if ATMIGHTY_MESSAGELOG_ENABLE == true
		if (returnCode > 0) //abstract hardware is already in use
		{
			logAllocFailUsed(instance->getHardwareStringRepresentation(), instance->getPinPort(), instance->getPinNumber(), owner, instance->getOwner());
		}
		else //physical hardware dependency could not be resolved
		{
			logAllocFailDependency(instance->getHardwareStringRepresentation(), instance->getPinPort(), instance->getPinNumber(), owner);
		}
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
		logFreeSuccess((*hardware)->getHardwareStringRepresentation(), (*hardware)->getPinPort(), (*hardware)->getPinNumber(), owner);
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

//Pins
template SpecificIoPin<'A',0>* AbstractHardwareManager::allocIoPin<'A',0>();
template SpecificIoPin<'A',1>* AbstractHardwareManager::allocIoPin<'A',1>();
template SpecificIoPin<'A',2>* AbstractHardwareManager::allocIoPin<'A',2>();
template SpecificIoPin<'A',3>* AbstractHardwareManager::allocIoPin<'A',3>();
template SpecificIoPin<'A',4>* AbstractHardwareManager::allocIoPin<'A',4>();
template SpecificIoPin<'A',5>* AbstractHardwareManager::allocIoPin<'A',5>();
template SpecificIoPin<'A',6>* AbstractHardwareManager::allocIoPin<'A',6>();
template SpecificIoPin<'A',7>* AbstractHardwareManager::allocIoPin<'A',7>();
template void AbstractHardwareManager::freeItem<AbstractIoPin>(AbstractIoPin **hardware);
template void AbstractHardwareManager::freeItem<GeneralIoPin>(GeneralIoPin **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'A',0>>(SpecificIoPin<'A',0> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'A',1>>(SpecificIoPin<'A',1> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'A',2>>(SpecificIoPin<'A',2> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'A',3>>(SpecificIoPin<'A',3> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'A',4>>(SpecificIoPin<'A',4> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'A',5>>(SpecificIoPin<'A',5> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'A',6>>(SpecificIoPin<'A',6> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'A',7>>(SpecificIoPin<'A',7> **hardware);
