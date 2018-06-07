/*!
 * This file implements the class \see GeneralIoPort
 */


#include <ATmighty/Ressources/Periphery/Physical/Ports.h>
#include <ATmighty/Ressources/Periphery/Utilities/IoPortAddresses.h>
#include "GeneralIoPort.h"
#include "ATmighty/Utilities/LUTs/HardwareOwnerID.h"
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"


GeneralIoPort::GeneralIoPort(char portChar) : owner(0), physicalReference(nullptr), portAddress(IoPortAddresses::CharToAddress(portChar))
{
	if (!IoPortAddresses::IsValidPortChar(portChar))
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
			physicalReference = PhysicalHardwareManager::Alloc<Port<'A'>>(OwnerID::DirectAbstraction);
			break;
		case 'B':
			physicalReference = PhysicalHardwareManager::Alloc<Port<'B'>>(OwnerID::DirectAbstraction);
			break;
		case 'C':
			physicalReference = PhysicalHardwareManager::Alloc<Port<'C'>>(OwnerID::DirectAbstraction);
			break;
		case 'D':
			physicalReference = PhysicalHardwareManager::Alloc<Port<'D'>>(OwnerID::DirectAbstraction);
			break;
		case 'E':
			physicalReference = PhysicalHardwareManager::Alloc<Port<'E'>>(OwnerID::DirectAbstraction);
			break;
		case 'F':
			physicalReference = PhysicalHardwareManager::Alloc<Port<'F'>>(OwnerID::DirectAbstraction);
			break;
		case 'G':
			physicalReference = PhysicalHardwareManager::Alloc<Port<'G'>>(OwnerID::DirectAbstraction);
			break;
		case 'H':
			physicalReference = PhysicalHardwareManager::Alloc<Port<'H'>>(OwnerID::DirectAbstraction);
			break;
		case 'J':
			physicalReference = PhysicalHardwareManager::Alloc<Port<'J'>>(OwnerID::DirectAbstraction);
			break;
		case 'K':
			physicalReference = PhysicalHardwareManager::Alloc<Port<'K'>>(OwnerID::DirectAbstraction);
			break;
		case 'L':
			physicalReference = PhysicalHardwareManager::Alloc<Port<'L'>>(OwnerID::DirectAbstraction);
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
		PhysicalHardwareManager::Free((Port<'A'>**)(&physicalReference));
		break;
	case 'B':
		PhysicalHardwareManager::Free((Port<'B'>**)(&physicalReference));
		break;
	case 'C':
		PhysicalHardwareManager::Free((Port<'C'>**)(&physicalReference));
		break;
	case 'D':
		PhysicalHardwareManager::Free((Port<'D'>**)(&physicalReference));
		break;
	case 'E':
		PhysicalHardwareManager::Free((Port<'E'>**)(&physicalReference));
		break;
	case 'F':
		PhysicalHardwareManager::Free((Port<'F'>**)(&physicalReference));
		break;
	case 'G':
		PhysicalHardwareManager::Free((Port<'G'>**)(&physicalReference));
		break;
	case 'H':
		PhysicalHardwareManager::Free((Port<'H'>**)(&physicalReference));
		break;
	case 'J':
		PhysicalHardwareManager::Free((Port<'J'>**)(&physicalReference));
		break;
	case 'K':
		PhysicalHardwareManager::Free((Port<'K'>**)(&physicalReference));
		break;
	case 'L':
		PhysicalHardwareManager::Free((Port<'L'>**)(&physicalReference));
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
	return IoPortAddresses::AddressToChar((uint8_t*)portAddress);
}
