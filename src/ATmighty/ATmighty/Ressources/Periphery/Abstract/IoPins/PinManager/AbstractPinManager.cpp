/*!
 * This file implements the class \see AbstractPinManager
 */


#include <ATmighty/Ressources/Periphery/Physical/Ports.h>
#include "AbstractPinManager.h"
#include <util/atomic.h>
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"
#include "ATmighty/Utilities/LUTs/HardwareOwnerID.h"


//Implementation of the class AbstractPinManager

template <char PortChar> uint8_t AbstractPinManager<PortChar>::UsageMask = 0;

template <char PortChar> Port<PortChar>* AbstractPinManager<PortChar>::PhysicalReference = nullptr;

template <char PortChar> int8_t AbstractPinManager<PortChar>::AllocPin(uint8_t pin)
{
	if (!PhysicalReference) //need to allocate physical IoPort?
	{
		PhysicalReference = PhysicalHardwareManager::Alloc<Port<PortChar>>(OwnerID::IndirectAbstraction);
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

template <char PortChar> void AbstractPinManager<PortChar>::FreePin(uint8_t pin)
{
	UsageMask &= (~(1<<pin));

	if (!UsageMask && PhysicalReference) //entire port free and physical IoPort still allocated?
	{
		PhysicalHardwareManager::Free(&PhysicalReference);
	}
}

//Explizit Instantiations of this template class
#if defined (__AVR_ATmega2560__)
template class AbstractPinManager<'A'>;
template class AbstractPinManager<'B'>;
template class AbstractPinManager<'C'>;
template class AbstractPinManager<'D'>;
template class AbstractPinManager<'E'>;
template class AbstractPinManager<'F'>;
template class AbstractPinManager<'G'>;
template class AbstractPinManager<'H'>;
template class AbstractPinManager<'J'>;
template class AbstractPinManager<'K'>;
template class AbstractPinManager<'L'>;
#else
#  warning "AbstractPinManager was not configured for this µC."
#endif

