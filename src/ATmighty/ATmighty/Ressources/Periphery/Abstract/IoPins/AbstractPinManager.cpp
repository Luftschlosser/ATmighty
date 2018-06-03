/*!
 * This file implements the class \see AbstractPinManager
 */


#include "AbstractPinManager.h"
#include "ATmighty/Ressources/Periphery/Physical/IoPorts.h"
#include <util/atomic.h>
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"
#include "ATmighty/Utilities/LUTs/HardwareOwnerID.h"


template <class PhysicalPort> uint8_t AbstractPinManager<PhysicalPort>::UsageMask = 0;
template <class PhysicalPort> PhysicalPort* AbstractPinManager<PhysicalPort>::PhysicalReference = nullptr;


template <class PhysicalPort> int8_t AbstractPinManager<PhysicalPort>::AllocPin(uint8_t pin)
{
	if (!PhysicalReference) //need to allocate physical IoPort?
	{
		PhysicalReference = PhysicalHardwareManager::Alloc<PhysicalPort>(OwnerID::IndirectAbstraction);
		if (!PhysicalReference) //physical hardware could not be allocated?
		{
			return -1;
		}
	}


	int8_t retbuf;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		if (UsageMask & (1<<pin)) //Pin already in use?
		{
			retbuf = 1;
		}
		else //sucessfull allocation:
		{
			UsageMask |= (1<<pin);
			retbuf = 0;
		}
	}

	return retbuf;
}

template <class PhysicalPort> void AbstractPinManager<PhysicalPort>::FreePin(uint8_t pin)
{
	UsageMask &= (~(1<<pin));

	if (!UsageMask && PhysicalReference) //entire port free and physical IoPort still allocated?
	{
		PhysicalHardwareManager::Free(&PhysicalReference);
	}
}


//Explizit Instantiations of this template class
template class AbstractPinManager<PortA>;
template class AbstractPinManager<PortB>;
template class AbstractPinManager<PortC>;
template class AbstractPinManager<PortD>;
template class AbstractPinManager<PortE>;
template class AbstractPinManager<PortF>;
