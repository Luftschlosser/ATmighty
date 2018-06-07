/*!
 * This file implements the IoPin- allocation and freeing-methods of class \see AbstractHardwareManager
 */

#include "AbstractHardwareManager.h"
#include <util/atomic.h>


template<class ToFree> void AbstractHardwareManager::Free(ToFree **hardware, int8_t owner)
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


//AbstractHardwareManager-Implementation

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
	AbstractHardwareManager::Free<AbstractIoPin>((AbstractIoPin**) hardware, owner);
}


//The explizitely instantiated versions of the template-functions
template SpecificIoPin<'A',0>* AbstractHardwareManager::allocIoPin<'A',0>();
template SpecificIoPin<'A',1>* AbstractHardwareManager::allocIoPin<'A',1>();
template SpecificIoPin<'A',2>* AbstractHardwareManager::allocIoPin<'A',2>();
template SpecificIoPin<'A',3>* AbstractHardwareManager::allocIoPin<'A',3>();
template SpecificIoPin<'A',4>* AbstractHardwareManager::allocIoPin<'A',4>();
template SpecificIoPin<'A',5>* AbstractHardwareManager::allocIoPin<'A',5>();
template SpecificIoPin<'A',6>* AbstractHardwareManager::allocIoPin<'A',6>();
template SpecificIoPin<'A',7>* AbstractHardwareManager::allocIoPin<'A',7>();
template SpecificIoPin<'B',0>* AbstractHardwareManager::allocIoPin<'B',0>();
template SpecificIoPin<'B',1>* AbstractHardwareManager::allocIoPin<'B',1>();
template SpecificIoPin<'B',2>* AbstractHardwareManager::allocIoPin<'B',2>();
template SpecificIoPin<'B',3>* AbstractHardwareManager::allocIoPin<'B',3>();
template SpecificIoPin<'B',4>* AbstractHardwareManager::allocIoPin<'B',4>();
template SpecificIoPin<'B',5>* AbstractHardwareManager::allocIoPin<'B',5>();
template SpecificIoPin<'B',6>* AbstractHardwareManager::allocIoPin<'B',6>();
template SpecificIoPin<'B',7>* AbstractHardwareManager::allocIoPin<'B',7>();
template SpecificIoPin<'C',0>* AbstractHardwareManager::allocIoPin<'C',0>();
template SpecificIoPin<'C',1>* AbstractHardwareManager::allocIoPin<'C',1>();
template SpecificIoPin<'C',2>* AbstractHardwareManager::allocIoPin<'C',2>();
template SpecificIoPin<'C',3>* AbstractHardwareManager::allocIoPin<'C',3>();
template SpecificIoPin<'C',4>* AbstractHardwareManager::allocIoPin<'C',4>();
template SpecificIoPin<'C',5>* AbstractHardwareManager::allocIoPin<'C',5>();
template SpecificIoPin<'C',6>* AbstractHardwareManager::allocIoPin<'C',6>();
template SpecificIoPin<'C',7>* AbstractHardwareManager::allocIoPin<'C',7>();
template SpecificIoPin<'D',0>* AbstractHardwareManager::allocIoPin<'D',0>();
template SpecificIoPin<'D',1>* AbstractHardwareManager::allocIoPin<'D',1>();
template SpecificIoPin<'D',2>* AbstractHardwareManager::allocIoPin<'D',2>();
template SpecificIoPin<'D',3>* AbstractHardwareManager::allocIoPin<'D',3>();
template SpecificIoPin<'D',4>* AbstractHardwareManager::allocIoPin<'D',4>();
template SpecificIoPin<'D',5>* AbstractHardwareManager::allocIoPin<'D',5>();
template SpecificIoPin<'D',6>* AbstractHardwareManager::allocIoPin<'D',6>();
template SpecificIoPin<'D',7>* AbstractHardwareManager::allocIoPin<'D',7>();
template SpecificIoPin<'E',0>* AbstractHardwareManager::allocIoPin<'E',0>();
template SpecificIoPin<'E',1>* AbstractHardwareManager::allocIoPin<'E',1>();
template SpecificIoPin<'E',2>* AbstractHardwareManager::allocIoPin<'E',2>();
template SpecificIoPin<'E',3>* AbstractHardwareManager::allocIoPin<'E',3>();
template SpecificIoPin<'E',4>* AbstractHardwareManager::allocIoPin<'E',4>();
template SpecificIoPin<'E',5>* AbstractHardwareManager::allocIoPin<'E',5>();
template SpecificIoPin<'E',6>* AbstractHardwareManager::allocIoPin<'E',6>();
template SpecificIoPin<'E',7>* AbstractHardwareManager::allocIoPin<'E',7>();
template SpecificIoPin<'F',0>* AbstractHardwareManager::allocIoPin<'F',0>();
template SpecificIoPin<'F',1>* AbstractHardwareManager::allocIoPin<'F',1>();
template SpecificIoPin<'F',2>* AbstractHardwareManager::allocIoPin<'F',2>();
template SpecificIoPin<'F',3>* AbstractHardwareManager::allocIoPin<'F',3>();
template SpecificIoPin<'F',4>* AbstractHardwareManager::allocIoPin<'F',4>();
template SpecificIoPin<'F',5>* AbstractHardwareManager::allocIoPin<'F',5>();
template SpecificIoPin<'F',6>* AbstractHardwareManager::allocIoPin<'F',6>();
template SpecificIoPin<'F',7>* AbstractHardwareManager::allocIoPin<'F',7>();
template SpecificIoPin<'G',0>* AbstractHardwareManager::allocIoPin<'G',0>();
template SpecificIoPin<'G',1>* AbstractHardwareManager::allocIoPin<'G',1>();
template SpecificIoPin<'G',2>* AbstractHardwareManager::allocIoPin<'G',2>();
template SpecificIoPin<'G',3>* AbstractHardwareManager::allocIoPin<'G',3>();
template SpecificIoPin<'G',4>* AbstractHardwareManager::allocIoPin<'G',4>();
template SpecificIoPin<'G',5>* AbstractHardwareManager::allocIoPin<'G',5>();
template SpecificIoPin<'G',6>* AbstractHardwareManager::allocIoPin<'G',6>();
template SpecificIoPin<'G',7>* AbstractHardwareManager::allocIoPin<'G',7>();
template SpecificIoPin<'H',0>* AbstractHardwareManager::allocIoPin<'H',0>();
template SpecificIoPin<'H',1>* AbstractHardwareManager::allocIoPin<'H',1>();
template SpecificIoPin<'H',2>* AbstractHardwareManager::allocIoPin<'H',2>();
template SpecificIoPin<'H',3>* AbstractHardwareManager::allocIoPin<'H',3>();
template SpecificIoPin<'H',4>* AbstractHardwareManager::allocIoPin<'H',4>();
template SpecificIoPin<'H',5>* AbstractHardwareManager::allocIoPin<'H',5>();
template SpecificIoPin<'H',6>* AbstractHardwareManager::allocIoPin<'H',6>();
template SpecificIoPin<'H',7>* AbstractHardwareManager::allocIoPin<'H',7>();
template SpecificIoPin<'J',0>* AbstractHardwareManager::allocIoPin<'J',0>();
template SpecificIoPin<'J',1>* AbstractHardwareManager::allocIoPin<'J',1>();
template SpecificIoPin<'J',2>* AbstractHardwareManager::allocIoPin<'J',2>();
template SpecificIoPin<'J',3>* AbstractHardwareManager::allocIoPin<'J',3>();
template SpecificIoPin<'J',4>* AbstractHardwareManager::allocIoPin<'J',4>();
template SpecificIoPin<'J',5>* AbstractHardwareManager::allocIoPin<'J',5>();
template SpecificIoPin<'J',6>* AbstractHardwareManager::allocIoPin<'J',6>();
template SpecificIoPin<'J',7>* AbstractHardwareManager::allocIoPin<'J',7>();
template SpecificIoPin<'K',0>* AbstractHardwareManager::allocIoPin<'K',0>();
template SpecificIoPin<'K',1>* AbstractHardwareManager::allocIoPin<'K',1>();
template SpecificIoPin<'K',2>* AbstractHardwareManager::allocIoPin<'K',2>();
template SpecificIoPin<'K',3>* AbstractHardwareManager::allocIoPin<'K',3>();
template SpecificIoPin<'K',4>* AbstractHardwareManager::allocIoPin<'K',4>();
template SpecificIoPin<'K',5>* AbstractHardwareManager::allocIoPin<'K',5>();
template SpecificIoPin<'K',6>* AbstractHardwareManager::allocIoPin<'K',6>();
template SpecificIoPin<'K',7>* AbstractHardwareManager::allocIoPin<'K',7>();
template SpecificIoPin<'L',0>* AbstractHardwareManager::allocIoPin<'L',0>();
template SpecificIoPin<'L',1>* AbstractHardwareManager::allocIoPin<'L',1>();
template SpecificIoPin<'L',2>* AbstractHardwareManager::allocIoPin<'L',2>();
template SpecificIoPin<'L',3>* AbstractHardwareManager::allocIoPin<'L',3>();
template SpecificIoPin<'L',4>* AbstractHardwareManager::allocIoPin<'L',4>();
template SpecificIoPin<'L',5>* AbstractHardwareManager::allocIoPin<'L',5>();
template SpecificIoPin<'L',6>* AbstractHardwareManager::allocIoPin<'L',6>();
template SpecificIoPin<'L',7>* AbstractHardwareManager::allocIoPin<'L',7>();
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
template void AbstractHardwareManager::freeItem<SpecificIoPin<'B',0>>(SpecificIoPin<'B',0> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'B',1>>(SpecificIoPin<'B',1> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'B',2>>(SpecificIoPin<'B',2> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'B',3>>(SpecificIoPin<'B',3> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'B',4>>(SpecificIoPin<'B',4> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'B',5>>(SpecificIoPin<'B',5> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'B',6>>(SpecificIoPin<'B',6> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'B',7>>(SpecificIoPin<'B',7> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'C',0>>(SpecificIoPin<'C',0> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'C',1>>(SpecificIoPin<'C',1> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'C',2>>(SpecificIoPin<'C',2> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'C',3>>(SpecificIoPin<'C',3> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'C',4>>(SpecificIoPin<'C',4> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'C',5>>(SpecificIoPin<'C',5> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'C',6>>(SpecificIoPin<'C',6> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'C',7>>(SpecificIoPin<'C',7> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'D',0>>(SpecificIoPin<'D',0> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'D',1>>(SpecificIoPin<'D',1> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'D',2>>(SpecificIoPin<'D',2> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'D',3>>(SpecificIoPin<'D',3> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'D',4>>(SpecificIoPin<'D',4> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'D',5>>(SpecificIoPin<'D',5> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'D',6>>(SpecificIoPin<'D',6> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'D',7>>(SpecificIoPin<'D',7> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'E',0>>(SpecificIoPin<'E',0> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'E',1>>(SpecificIoPin<'E',1> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'E',2>>(SpecificIoPin<'E',2> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'E',3>>(SpecificIoPin<'E',3> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'E',4>>(SpecificIoPin<'E',4> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'E',5>>(SpecificIoPin<'E',5> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'E',6>>(SpecificIoPin<'E',6> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'E',7>>(SpecificIoPin<'E',7> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'F',0>>(SpecificIoPin<'F',0> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'F',1>>(SpecificIoPin<'F',1> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'F',2>>(SpecificIoPin<'F',2> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'F',3>>(SpecificIoPin<'F',3> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'F',4>>(SpecificIoPin<'F',4> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'F',5>>(SpecificIoPin<'F',5> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'F',6>>(SpecificIoPin<'F',6> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'F',7>>(SpecificIoPin<'F',7> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'G',0>>(SpecificIoPin<'G',0> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'G',1>>(SpecificIoPin<'G',1> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'G',2>>(SpecificIoPin<'G',2> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'G',3>>(SpecificIoPin<'G',3> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'G',4>>(SpecificIoPin<'G',4> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'G',5>>(SpecificIoPin<'G',5> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'G',6>>(SpecificIoPin<'G',6> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'G',7>>(SpecificIoPin<'G',7> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'H',0>>(SpecificIoPin<'H',0> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'H',1>>(SpecificIoPin<'H',1> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'H',2>>(SpecificIoPin<'H',2> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'H',3>>(SpecificIoPin<'H',3> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'H',4>>(SpecificIoPin<'H',4> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'H',5>>(SpecificIoPin<'H',5> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'H',6>>(SpecificIoPin<'H',6> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'H',7>>(SpecificIoPin<'H',7> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'J',0>>(SpecificIoPin<'J',0> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'J',1>>(SpecificIoPin<'J',1> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'J',2>>(SpecificIoPin<'J',2> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'J',3>>(SpecificIoPin<'J',3> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'J',4>>(SpecificIoPin<'J',4> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'J',5>>(SpecificIoPin<'J',5> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'J',6>>(SpecificIoPin<'J',6> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'J',7>>(SpecificIoPin<'J',7> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'K',0>>(SpecificIoPin<'K',0> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'K',1>>(SpecificIoPin<'K',1> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'K',2>>(SpecificIoPin<'K',2> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'K',3>>(SpecificIoPin<'K',3> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'K',4>>(SpecificIoPin<'K',4> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'K',5>>(SpecificIoPin<'K',5> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'K',6>>(SpecificIoPin<'K',6> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'K',7>>(SpecificIoPin<'K',7> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'L',0>>(SpecificIoPin<'L',0> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'L',1>>(SpecificIoPin<'L',1> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'L',2>>(SpecificIoPin<'L',2> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'L',3>>(SpecificIoPin<'L',3> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'L',4>>(SpecificIoPin<'L',4> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'L',5>>(SpecificIoPin<'L',5> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'L',6>>(SpecificIoPin<'L',6> **hardware);
template void AbstractHardwareManager::freeItem<SpecificIoPin<'L',7>>(SpecificIoPin<'L',7> **hardware);
template void AbstractHardwareManager::Free<AbstractIoPin>(AbstractIoPin** hardware, int8_t owner);
