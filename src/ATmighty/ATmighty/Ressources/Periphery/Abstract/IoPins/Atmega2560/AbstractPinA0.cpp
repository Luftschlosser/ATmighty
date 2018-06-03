/*!
 * This headerfile is µC-dependent, it should only be included indirectly via "AbstractHardwareManager.h".
 * This file implements the class \see AbstractPinA0
 */


#include "AbstractPinA0.h"
#include <avr/io.h>
#include "ATmighty/Ressources/Periphery/Abstract/IoPins/AbstractPinManager.h"
#include "ATmighty/Ressources/Periphery/Physical/IoPorts.h"


int8_t AbstractPinA0::init(int8_t ownerId)
{
	if (!AbstractIoPin::init(ownerId))
	{
		int8_t allocResult = AbstractPinManager<PortA>::AllocPin(0);
		if (allocResult)
		{
			AbstractIoPin::exit();
			return allocResult;
		}
		else
		{

			return 0;
		}
	}
	else
	{
		return 1;
	}
}

void AbstractPinA0::exit()
{
	AbstractPinManager<PortA>::FreePin(0);
	AbstractIoPin::exit();
}

void AbstractPinA0::setDirection(AbstractPinA0::DataDirection direction)
{
	if (direction == DataDirection::Output)
	{
		DDRA |= 1<<0; //compiler will optimize to "sbi"
	}
	else
	{
		DDRA &= ~(1<<0); //compiler will optimize to "cli"
	}
}

void AbstractPinA0::set(bool value)
{
	if (value)
	{
		PORTA |= 1<<0; //compiler will optimize to "sbi"
	}
	else
	{
		PORTA &= ~(1<<0); //compiler will optimize to "cli"
	}
}

void AbstractPinA0::toggle()
{
	PINA |= 1<<0; //compiler will optimize to "sbi"
}

AbstractPinA0::DataDirection AbstractPinA0::getDirection()
{
	return (DataDirection)(DDRA & (1<<0));
}

bool AbstractPinA0::read()
{
	return (bool)(PINA & (1<<0));
}
