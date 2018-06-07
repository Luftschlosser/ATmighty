/*!
 * This file implements the class \see SpecificIoPort
 */


#include <ATmighty/Ressources/Periphery/Physical/Ports.h>
#include <ATmighty/Ressources/Periphery/Utilities/IoPortAddresses.h>
#include "SpecificIoPort.h"
#include "ATmighty/Utilities/LUTs/HardwareOwnerID.h"
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"


template<char PortChar> int8_t SpecificIoPort<PortChar>::Owner = 0;

template<char PortChar> int8_t SpecificIoPort<PortChar>::init(int8_t ownerId)
{
	if (Owner == 0)
	{
		physicalReference = PhysicalHardwareManager::Alloc<Port<'A'>>(OwnerID::DirectAbstraction);

		if (physicalReference != nullptr)
		{
			Owner = ownerId;
			return 0;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return 1;
	}
}

template<char PortChar> void SpecificIoPort<PortChar>::exit()
{
	PhysicalHardwareManager::Free((Port<PortChar>**)(&physicalReference));

	Owner = 0;
}

template<char PortChar> void SpecificIoPort<PortChar>::setDataDirectionMask(uint8_t mask)
{
	/*DDRX*/*(IoPortAddresses::CharToAddress(PortChar)+1) = mask; //compiler should be able to calculate all constants and optimize to single instruction
}

template<char PortChar> void SpecificIoPort<PortChar>::setData(uint8_t data)
{
	/*PORTX*/*(IoPortAddresses::CharToAddress(PortChar)+2) = data; //compiler should be able to calculate all constants and optimize to single instruction
}

template<char PortChar> void SpecificIoPort<PortChar>::applyPinToggleMask(uint8_t mask)
{
	/*PINX*/*(IoPortAddresses::CharToAddress(PortChar)) = mask; //compiler should be able to calculate all constants and optimize to single instruction
}

template<char PortChar> uint8_t SpecificIoPort<PortChar>::getDataDirections()
{
	return /*DDRX*/*(IoPortAddresses::CharToAddress(PortChar)+1); //compiler should be able to calculate all constants and optimize to single instruction
}

template<char PortChar> uint8_t SpecificIoPort<PortChar>::getPinValues()
{
	return /*PINX*/*(IoPortAddresses::CharToAddress(PortChar));
}


//Explizit template instantiations for this µC
template class SpecificIoPort<'A'>;
template class SpecificIoPort<'B'>;
template class SpecificIoPort<'C'>;
template class SpecificIoPort<'D'>;
template class SpecificIoPort<'E'>;
template class SpecificIoPort<'F'>;
template class SpecificIoPort<'G'>;
template class SpecificIoPort<'H'>;
template class SpecificIoPort<'J'>;
template class SpecificIoPort<'K'>;
template class SpecificIoPort<'L'>;
