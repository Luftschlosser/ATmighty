/*!
 * This file implements the class \see GeneralIoPin
 */


#include "GeneralIoPin.h"
#include "ATmighty/Ressources/Periphery/Abstract/IoPorts/IoPortData.h"
#include "ATmighty/Ressources/Periphery/Abstract/IoPins/PinManager/AbstractPinManager.h"
#include "ATmighty/Ressources/Periphery/Physical/IoPorts.h"


GeneralIoPin::GeneralIoPin(char portChar, uint8_t pinNumber) : owner(0), pin(pinNumber), portAddress(PortData::CharToAddress(portChar))
{
	if (!PortData::IsValidPortChar(portChar))
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
			allocResult = AbstractPinManager<PortA>::AllocPin(pin);
			break;
		case 'B':
			allocResult = AbstractPinManager<PortB>::AllocPin(pin);
			break;
		case 'C':
			allocResult = AbstractPinManager<PortC>::AllocPin(pin);
			break;
		case 'D':
			allocResult = AbstractPinManager<PortD>::AllocPin(pin);
			break;
		case 'E':
			allocResult = AbstractPinManager<PortE>::AllocPin(pin);
			break;
		case 'F':
			allocResult = AbstractPinManager<PortF>::AllocPin(pin);
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
		AbstractPinManager<PortA>::FreePin(pin);
		break;
	case 'B':
		AbstractPinManager<PortB>::FreePin(pin);
		break;
	case 'C':
		AbstractPinManager<PortC>::FreePin(pin);
		break;
	case 'D':
		AbstractPinManager<PortD>::FreePin(pin);
		break;
	case 'E':
		AbstractPinManager<PortE>::FreePin(pin);
		break;
	case 'F':
		AbstractPinManager<PortF>::FreePin(pin);
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
	return PortData::AddressToChar((uint8_t*)portAddress);
}
