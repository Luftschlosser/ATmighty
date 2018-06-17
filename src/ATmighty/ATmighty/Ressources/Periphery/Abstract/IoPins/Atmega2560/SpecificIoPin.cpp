/*!
 * This file implements the class \see SpecificIoPin
 */


#include <ATmighty/Ressources/Periphery/Abstract/IoPins/Atmega2560/SpecificIoPin.h>
#include <ATmighty/Ressources/Periphery/Physical/Ports.h>
#include <ATmighty/Ressources/Periphery/Utilities/IoPortAddresses.h>
#include "ATmighty/Ressources/Periphery/Abstract/IoPins/PinManager/AbstractPinManager.h"


template<char PortChar, uint8_t PinNumber> int8_t SpecificIoPin<PortChar, PinNumber>::Owner = 0;

template<char PortChar, uint8_t PinNumber> int8_t SpecificIoPin<PortChar, PinNumber>::init(int8_t ownerId)
{
	if (Owner == 0)
	{
		int8_t allocResult = AbstractPinManager<PortChar>::AllocPin(PinNumber);

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
	AbstractPinManager<PortChar>::FreePin(PinNumber);

	Owner = 0;
}

template<char PortChar, uint8_t PinNumber> void SpecificIoPin<PortChar, PinNumber>::setDirection(AbstractIoPin::DataDirection direction)
{
	if (direction == DataDirection::Output)
	{
		/*DDRX*/*(IoPortAddresses::CharToAddress(PortChar)+1) |= 1<<PinNumber; //compiler should be able to calculate all constants and optimize to "sbi"
	}
	else
	{
		/*DDRX*/*(IoPortAddresses::CharToAddress(PortChar)+1) &= ~(1<<PinNumber); //compiler should be able to calculate all constants and optimize to "cli"
	}
}

template<char PortChar, uint8_t PinNumber> void SpecificIoPin<PortChar, PinNumber>::set(bool value)
{
	if (value)
	{
		/*PORTX*/*(IoPortAddresses::CharToAddress(PortChar)+2) |= 1<<PinNumber; //compiler should be able to calculate all constants and optimize to "sbi"
	}
	else
	{
		/*PORTX*/*(IoPortAddresses::CharToAddress(PortChar)+2) &= ~(1<<PinNumber); //compiler should be able to calculate all constants and optimize to "cli"
	}
}

template<char PortChar, uint8_t PinNumber> void SpecificIoPin<PortChar, PinNumber>::toggle()
{
	/*PINX*/*(IoPortAddresses::CharToAddress(PortChar)) = 1<<PinNumber; //compiler should be able to calculate all constants and optimize to "sbi"
}

template<char PortChar, uint8_t PinNumber> AbstractIoPin::DataDirection SpecificIoPin<PortChar, PinNumber>::getDirection()
{
	return (DataDirection)(/*DDRX*/*(IoPortAddresses::CharToAddress(PortChar)+1) & (1<<PinNumber)); //compiler should be able to calculate all constants and optimize to single instruction
}

template<char PortChar, uint8_t PinNumber> bool SpecificIoPin<PortChar, PinNumber>::read()
{
	return (bool)(/*PINX*/*(IoPortAddresses::CharToAddress(PortChar)) & (1<<PinNumber)); //compiler should be able to calculate all constants and optimize to single instruction
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
template class SpecificIoPin<'B',0>;
template class SpecificIoPin<'B',1>;
template class SpecificIoPin<'B',2>;
template class SpecificIoPin<'B',3>;
template class SpecificIoPin<'B',4>;
template class SpecificIoPin<'B',5>;
template class SpecificIoPin<'B',6>;
template class SpecificIoPin<'B',7>;
template class SpecificIoPin<'C',0>;
template class SpecificIoPin<'C',1>;
template class SpecificIoPin<'C',2>;
template class SpecificIoPin<'C',3>;
template class SpecificIoPin<'C',4>;
template class SpecificIoPin<'C',5>;
template class SpecificIoPin<'C',6>;
template class SpecificIoPin<'C',7>;
template class SpecificIoPin<'D',0>;
template class SpecificIoPin<'D',1>;
template class SpecificIoPin<'D',2>;
template class SpecificIoPin<'D',3>;
template class SpecificIoPin<'D',4>;
template class SpecificIoPin<'D',5>;
template class SpecificIoPin<'D',6>;
template class SpecificIoPin<'D',7>;
template class SpecificIoPin<'E',0>;
template class SpecificIoPin<'E',1>;
template class SpecificIoPin<'E',2>;
template class SpecificIoPin<'E',3>;
template class SpecificIoPin<'E',4>;
template class SpecificIoPin<'E',5>;
template class SpecificIoPin<'E',6>;
template class SpecificIoPin<'E',7>;
template class SpecificIoPin<'F',0>;
template class SpecificIoPin<'F',1>;
template class SpecificIoPin<'F',2>;
template class SpecificIoPin<'F',3>;
template class SpecificIoPin<'F',4>;
template class SpecificIoPin<'F',5>;
template class SpecificIoPin<'F',6>;
template class SpecificIoPin<'F',7>;
template class SpecificIoPin<'G',0>;
template class SpecificIoPin<'G',1>;
template class SpecificIoPin<'G',2>;
template class SpecificIoPin<'G',3>;
template class SpecificIoPin<'G',4>;
template class SpecificIoPin<'G',5>;
template class SpecificIoPin<'G',6>;
template class SpecificIoPin<'G',7>;
template class SpecificIoPin<'H',0>;
template class SpecificIoPin<'H',1>;
template class SpecificIoPin<'H',2>;
template class SpecificIoPin<'H',3>;
template class SpecificIoPin<'H',4>;
template class SpecificIoPin<'H',5>;
template class SpecificIoPin<'H',6>;
template class SpecificIoPin<'H',7>;
template class SpecificIoPin<'J',0>;
template class SpecificIoPin<'J',1>;
template class SpecificIoPin<'J',2>;
template class SpecificIoPin<'J',3>;
template class SpecificIoPin<'J',4>;
template class SpecificIoPin<'J',5>;
template class SpecificIoPin<'J',6>;
template class SpecificIoPin<'J',7>;
template class SpecificIoPin<'K',0>;
template class SpecificIoPin<'K',1>;
template class SpecificIoPin<'K',2>;
template class SpecificIoPin<'K',3>;
template class SpecificIoPin<'K',4>;
template class SpecificIoPin<'K',5>;
template class SpecificIoPin<'K',6>;
template class SpecificIoPin<'K',7>;
template class SpecificIoPin<'L',0>;
template class SpecificIoPin<'L',1>;
template class SpecificIoPin<'L',2>;
template class SpecificIoPin<'L',3>;
template class SpecificIoPin<'L',4>;
template class SpecificIoPin<'L',5>;
template class SpecificIoPin<'L',6>;
template class SpecificIoPin<'L',7>;
