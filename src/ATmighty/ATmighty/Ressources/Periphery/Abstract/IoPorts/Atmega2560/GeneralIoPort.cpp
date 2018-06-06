/*!
 * This file implements the class \see GeneralIoPort
 */


#include "GeneralIoPort.h"
#include "ATmighty/Utilities/LUTs/HardwareOwnerID.h"
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"
#include "ATmighty/Ressources/Periphery/Physical/IoPorts.h"
#include "ATmighty/Ressources/Periphery/Abstract/IoPorts/IoPortData.h"


GeneralIoPort::GeneralIoPort(char portChar) : owner(0), physicalReference(nullptr), portAddress(PortData::CharToAddress(portChar))
{
	if (!PortData::IsValidPortChar(portChar))
	{
		physicalReference = (uint8_t*)(1); //set physicalReference != 0 to function as flag for an invalid portChar!
	}
}

int8_t GeneralIoPort::init(int8_t ownerId)
{
	if (physicalReference != nullptr) //functions as flag for an invalid portChar
	{
		return -1;
	}

	if (owner == 0)
	{
		switch (getCharCode())
		{
		case 'A':
			physicalReference = PhysicalHardwareManager::Alloc<PortA>(OwnerID::DirectAbstraction);
			break;
		case 'B':
			physicalReference = PhysicalHardwareManager::Alloc<PortB>(OwnerID::DirectAbstraction);
			break;
		case 'C':
			physicalReference = PhysicalHardwareManager::Alloc<PortC>(OwnerID::DirectAbstraction);
			break;
		case 'D':
			physicalReference = PhysicalHardwareManager::Alloc<PortD>(OwnerID::DirectAbstraction);
			break;
		case 'E':
			physicalReference = PhysicalHardwareManager::Alloc<PortE>(OwnerID::DirectAbstraction);
			break;
		case 'F':
			physicalReference = PhysicalHardwareManager::Alloc<PortF>(OwnerID::DirectAbstraction);
			break;
		}

		if (physicalReference != nullptr)
		{
			owner = ownerId;
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

void GeneralIoPort::exit()
{
	switch (getCharCode())
	{
	case 'A':
		PhysicalHardwareManager::Free((PortA**)(&physicalReference));
		break;
	case 'B':
		PhysicalHardwareManager::Free((PortB**)(&physicalReference));
		break;
	case 'C':
		PhysicalHardwareManager::Free((PortC**)(&physicalReference));
		break;
	case 'D':
		PhysicalHardwareManager::Free((PortD**)(&physicalReference));
		break;
	case 'E':
		PhysicalHardwareManager::Free((PortE**)(&physicalReference));
		break;
	case 'F':
		PhysicalHardwareManager::Free((PortF**)(&physicalReference));
		break;
	}

	owner = 0;
}

void GeneralIoPort::setDataDirectionMask(uint8_t mask)
{
	/*DDRX*/*(portAddress+1) = mask; //compiler should be able to calculate all constants and optimize to single instruction
}

void GeneralIoPort::setData(uint8_t data)
{
	/*PORTX*/*(portAddress+2) = data; //compiler should be able to calculate all constants and optimize to single instruction
}

void GeneralIoPort::applyPinToggleMask(uint8_t mask)
{
	/*PINX*/*(portAddress) = mask; //compiler should be able to calculate all constants and optimize to single instruction
}

uint8_t GeneralIoPort::getDataDirections()
{
	return /*DDRX*/*(portAddress+1); //compiler should be able to calculate all constants and optimize to single instruction
}

uint8_t GeneralIoPort::getPinValues()
{
	return /*PINX*/*(portAddress);
}

char GeneralIoPort::getCharCode()
{
	return PortData::AddressToChar((uint8_t*)portAddress);
}
