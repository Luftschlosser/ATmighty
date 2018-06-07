/*!
 * This file implements the class \see GeneralIoPin
 */


#include <ATmighty/Ressources/Periphery/Physical/Ports.h>
#include <ATmighty/Ressources/Periphery/Utilities/IoPortAddresses.h>
#include "GeneralIoPin.h"
#include "ATmighty/Ressources/Periphery/Abstract/IoPins/PinManager/AbstractPinManager.h"


GeneralIoPin::GeneralIoPin(char portChar, uint8_t pinNumber) : owner(0), pin(pinNumber), portAddress(IoPortAddresses::CharToAddress(portChar))
{
	if (!IoPortAddresses::IsValidPortChar(portChar))
	{
		pin = 8; //if the port is invalid, assign an invalid pin-number to prevent it from being used.
	}
}

int8_t GeneralIoPin::init(int8_t ownerId)
{
	if (pin > 7)
	{
		return -1;
	}

	if (owner == 0)
	{
		int8_t allocResult;

		switch (getPinPort())
		{
		case 'A':
			allocResult = AbstractPinManager<'A'>::AllocPin(pin);
			break;
		case 'B':
			allocResult = AbstractPinManager<'B'>::AllocPin(pin);
			break;
		case 'C':
			allocResult = AbstractPinManager<'C'>::AllocPin(pin);
			break;
		case 'D':
			allocResult = AbstractPinManager<'D'>::AllocPin(pin);
			break;
		case 'E':
			allocResult = AbstractPinManager<'E'>::AllocPin(pin);
			break;
		case 'F':
			allocResult = AbstractPinManager<'F'>::AllocPin(pin);
			break;
		case 'G':
			allocResult = AbstractPinManager<'G'>::AllocPin(pin);
			break;
		case 'H':
			allocResult = AbstractPinManager<'H'>::AllocPin(pin);
			break;
		case 'J':
			allocResult = AbstractPinManager<'J'>::AllocPin(pin);
			break;
		case 'K':
			allocResult = AbstractPinManager<'K'>::AllocPin(pin);
			break;
		case 'L':
			allocResult = AbstractPinManager<'L'>::AllocPin(pin);
			break;
		default:
			allocResult = -1;
		}

		if (allocResult)
		{
			return allocResult;
		}
		else
		{
			owner = ownerId;
			return 0;
		}
	}
	else
	{
		return 1;
	}
}

void GeneralIoPin::exit()
{
	switch (getPinPort())
	{
	case 'A':
		AbstractPinManager<'A'>::FreePin(pin);
		break;
	case 'B':
		AbstractPinManager<'B'>::FreePin(pin);
		break;
	case 'C':
		AbstractPinManager<'C'>::FreePin(pin);
		break;
	case 'D':
		AbstractPinManager<'D'>::FreePin(pin);
		break;
	case 'E':
		AbstractPinManager<'E'>::FreePin(pin);
		break;
	case 'F':
		AbstractPinManager<'F'>::FreePin(pin);
		break;
	case 'G':
		AbstractPinManager<'G'>::FreePin(pin);
		break;
	case 'H':
		AbstractPinManager<'H'>::FreePin(pin);
		break;
	case 'J':
		AbstractPinManager<'J'>::FreePin(pin);
		break;
	case 'K':
		AbstractPinManager<'K'>::FreePin(pin);
		break;
	case 'L':
		AbstractPinManager<'L'>::FreePin(pin);
		break;
	}

	owner = 0;
}

void GeneralIoPin::setDirection(GeneralIoPin::DataDirection direction)
{
	if (direction == DataDirection::Output)
	{
		/*DDRX*/*(portAddress+1) |= 1<<pin;
	}
	else
	{
		/*DDRX*/*(portAddress+1) &= ~(1<<pin);
	}
}

void GeneralIoPin::set(bool value)
{
	if (value)
	{
		/*PORTX*/*(portAddress+2) |= 1<<pin;
	}
	else
	{
		/*PORTX*/*(portAddress+2) &= ~(1<<pin);
	}
}

void GeneralIoPin::toggle()
{
	/*PINX*/*(portAddress) |= 1<<pin;
}

GeneralIoPin::DataDirection GeneralIoPin::getDirection()
{
	return (DataDirection)(/*DDRX*/*(portAddress+1) & (1<<pin));
}

bool GeneralIoPin::read()
{
	return (bool)(/*PINX*/*(portAddress) & (1<<pin));
}

char GeneralIoPin::getPinPort()
{
	//double casting portAddress, because it throws an compiler-error when casting a pointer directly into an uint8_t
	return IoPortAddresses::AddressToChar((uint8_t*)portAddress);
}
