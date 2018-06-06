/*!
 * This file implements the class \see SpecificIoPin
 */


#include <ATmighty/Ressources/Periphery/Abstract/IoPins/Atmega2560/SpecificIoPin.h>
#include "ATmighty/Ressources/Periphery/Abstract/IoPorts/IoPortData.h"
#include "ATmighty/Ressources/Periphery/Abstract/IoPins/PinManager/AbstractPinManager.h"
#include "ATmighty/Ressources/Periphery/Physical/IoPorts.h"


template<char PortChar, uint8_t PinNumber> int8_t SpecificIoPin<PortChar, PinNumber>::Owner = 0;

template<char PortChar, uint8_t PinNumber> int8_t SpecificIoPin<PortChar, PinNumber>::init(int8_t ownerId)
{
	if (Owner == 0)
	{
		int8_t allocResult;

		if (PortChar == 'A')
			allocResult = AbstractPinManager<PortA>::AllocPin(PinNumber);
		else if (PortChar == 'B')
			allocResult = AbstractPinManager<PortB>::AllocPin(PinNumber);
		else if (PortChar == 'C')
			allocResult = AbstractPinManager<PortC>::AllocPin(PinNumber);
		else if (PortChar == 'D')
			allocResult = AbstractPinManager<PortD>::AllocPin(PinNumber);
		else if (PortChar == 'E')
			allocResult = AbstractPinManager<PortE>::AllocPin(PinNumber);
		else if (PortChar == 'F')
			allocResult = AbstractPinManager<PortF>::AllocPin(PinNumber);
		else
			allocResult = -1;

		if (allocResult)
		{
			return allocResult;
		}
		else
		{
			Owner = ownerId;
			return 0;
		}
	}
	else
	{
		return 1;
	}
}

template<char PortChar, uint8_t PinNumber> void SpecificIoPin<PortChar, PinNumber>::exit()
{
	if (PortChar == 'A')
		AbstractPinManager<PortA>::FreePin(PinNumber);
	else if (PortChar == 'B')
		AbstractPinManager<PortB>::FreePin(PinNumber);
	else if (PortChar == 'C')
		AbstractPinManager<PortC>::FreePin(PinNumber);
	else if (PortChar == 'D')
		AbstractPinManager<PortD>::FreePin(PinNumber);
	else if (PortChar == 'E')
		AbstractPinManager<PortE>::FreePin(PinNumber);
	else if (PortChar == 'F')
		AbstractPinManager<PortF>::FreePin(PinNumber);

	Owner = 0;
}

template<char PortChar, uint8_t PinNumber> void SpecificIoPin<PortChar, PinNumber>::setDirection(AbstractIoPin::DataDirection direction)
{
	if (direction == DataDirection::Output)
	{
		/*DDRX*/*(PortData::CharToAddress(PortChar)+1) |= 1<<PinNumber; //compiler should be able to calculate all constants and optimize to "sbi"
	}
	else
	{
		/*DDRX*/*(PortData::CharToAddress(PortChar)+1) &= ~(1<<PinNumber); //compiler should be able to calculate all constants and optimize to "cli"
	}
}

template<char PortChar, uint8_t PinNumber> void SpecificIoPin<PortChar, PinNumber>::set(bool value)
{
	if (value)
	{
		/*PORTX*/*(PortData::CharToAddress(PortChar)+2) |= 1<<PinNumber; //compiler should be able to calculate all constants and optimize to "sbi"
	}
	else
	{
		/*PORTX*/*(PortData::CharToAddress(PortChar)+2) &= ~(1<<PinNumber); //compiler should be able to calculate all constants and optimize to "cli"
	}
}

template<char PortChar, uint8_t PinNumber> void SpecificIoPin<PortChar, PinNumber>::toggle()
{
	/*PINX*/*(PortData::CharToAddress(PortChar)) |= 1<<PinNumber; //compiler should be able to calculate all constants and optimize to "sbi"
}

template<char PortChar, uint8_t PinNumber> AbstractIoPin::DataDirection SpecificIoPin<PortChar, PinNumber>::getDirection()
{
	return (DataDirection)(/*DDRX*/*(PortData::CharToAddress(PortChar)+1) & (1<<PinNumber)); //compiler should be able to calculate all constants and optimize to single instruction
}

template<char PortChar, uint8_t PinNumber> bool SpecificIoPin<PortChar, PinNumber>::read()
{
	return (bool)(/*PINX*/*(PortData::CharToAddress(PortChar)) & (1<<PinNumber)); //compiler should be able to calculate all constants and optimize to single instruction
}


//Explizit template instantiations for this µC
template class SpecificIoPin<'A',0>;
template class SpecificIoPin<'A',1>;
template class SpecificIoPin<'A',2>;
template class SpecificIoPin<'A',3>;
template class SpecificIoPin<'A',4>;
template class SpecificIoPin<'A',5>;
template class SpecificIoPin<'A',6>;
template class SpecificIoPin<'A',7>;
