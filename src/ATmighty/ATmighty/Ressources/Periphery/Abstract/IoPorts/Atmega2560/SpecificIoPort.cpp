/*!
 * This file implements the class \see SpecificIoPort
 */


#include "SpecificIoPort.h"
#include "ATmighty/Utilities/LUTs/HardwareOwnerID.h"
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"
#include "ATmighty/Ressources/Periphery/Physical/IoPorts.h"
#include "ATmighty/Ressources/Periphery/Abstract/IoPorts/IoPortData.h"


template<char PortChar> int8_t SpecificIoPort<PortChar>::Owner = 0;

template<char PortChar> int8_t SpecificIoPort<PortChar>::init(int8_t ownerId)
{
	if (Owner == 0)
	{
		if (PortChar == 'A')
			physicalReference = PhysicalHardwareManager::Alloc<PortA>(OwnerID::DirectAbstraction);
		else if (PortChar == 'B')
			physicalReference = PhysicalHardwareManager::Alloc<PortB>(OwnerID::DirectAbstraction);
		else if (PortChar == 'C')
			physicalReference = PhysicalHardwareManager::Alloc<PortC>(OwnerID::DirectAbstraction);
		else if (PortChar == 'D')
			physicalReference = PhysicalHardwareManager::Alloc<PortD>(OwnerID::DirectAbstraction);
		else if (PortChar == 'E')
			physicalReference = PhysicalHardwareManager::Alloc<PortE>(OwnerID::DirectAbstraction);
		else if (PortChar == 'F')
			physicalReference = PhysicalHardwareManager::Alloc<PortF>(OwnerID::DirectAbstraction);

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
	if (PortChar == 'A')
		PhysicalHardwareManager::Free((PortA**)(&physicalReference));
	else if (PortChar == 'B')
		PhysicalHardwareManager::Free((PortB**)(&physicalReference));
	else if (PortChar == 'C')
		PhysicalHardwareManager::Free((PortC**)(&physicalReference));
	else if (PortChar == 'D')
		PhysicalHardwareManager::Free((PortD**)(&physicalReference));
	else if (PortChar == 'E')
		PhysicalHardwareManager::Free((PortE**)(&physicalReference));
	else if (PortChar == 'F')
		PhysicalHardwareManager::Free((PortF**)(&physicalReference));

	Owner = 0;
}

template<char PortChar> void SpecificIoPort<PortChar>::setDataDirectionMask(uint8_t mask)
{
	/*DDRX*/*(PortData::CharToAddress(PortChar)+1) = mask; //compiler should be able to calculate all constants and optimize to single instruction
}

template<char PortChar> void SpecificIoPort<PortChar>::setData(uint8_t data)
{
	/*PORTX*/*(PortData::CharToAddress(PortChar)+2) = data; //compiler should be able to calculate all constants and optimize to single instruction
}

template<char PortChar> void SpecificIoPort<PortChar>::applyPinToggleMask(uint8_t mask)
{
	/*PINX*/*(PortData::CharToAddress(PortChar)) = mask; //compiler should be able to calculate all constants and optimize to single instruction
}

template<char PortChar> uint8_t SpecificIoPort<PortChar>::getDataDirections()
{
	return /*DDRX*/*(PortData::CharToAddress(PortChar)+1); //compiler should be able to calculate all constants and optimize to single instruction
}

template<char PortChar> uint8_t SpecificIoPort<PortChar>::getPinValues()
{
	return /*PINX*/*(PortData::CharToAddress(PortChar));
}


//Explizit template instantiations for this µC
template class SpecificIoPort<'A'>;
template class SpecificIoPort<'B'>;
template class SpecificIoPort<'C'>;
template class SpecificIoPort<'D'>;
template class SpecificIoPort<'E'>;
template class SpecificIoPort<'F'>;
