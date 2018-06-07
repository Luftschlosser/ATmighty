/*!
 * This file implements the class \see Port
 */


#include <avr/pgmspace.h>
#include <ATmighty/Ressources/Periphery/Physical/Ports/Atmega2560/Port.h>


template<char PortChar> PGM_P Port<PortChar>::GetHardwareStringRepresentation()
{
	//compiler-should optimize for each case:
	if (PortChar == 'A')
	{
		static const char me[] PROGMEM = "PortA";
		return me;
	}
	else if (PortChar == 'B')
	{
		static const char me[] PROGMEM = "PortB";
		return me;
	}
	else if (PortChar == 'C')
	{
		static const char me[] PROGMEM = "PortC";
		return me;
	}
	else if (PortChar == 'D')
	{
		static const char me[] PROGMEM = "PortD";
		return me;
	}
	else if (PortChar == 'E')
	{
		static const char me[] PROGMEM = "PortE";
		return me;
	}
	else if (PortChar == 'F')
	{
		static const char me[] PROGMEM = "PortF";
		return me;
	}
	else if (PortChar == 'G')
	{
		static const char me[] PROGMEM = "PortG";
		return me;
	}
	else if (PortChar == 'H')
	{
		static const char me[] PROGMEM = "PortH";
		return me;
	}
	else if (PortChar == 'J')
	{
		static const char me[] PROGMEM = "PortJ";
		return me;
	}
	else if (PortChar == 'K')
	{
		static const char me[] PROGMEM = "PortK";
		return me;
	}
	else if (PortChar == 'L')
	{
		static const char me[] PROGMEM = "PortL";
		return me;
	}
	else
	{
		static const char me[] PROGMEM = "PortX";
		return me;
	}
}


//Explizit template instantiations for this µC
template class Port<'A'>;
template class Port<'B'>;
template class Port<'C'>;
template class Port<'D'>;
template class Port<'E'>;
template class Port<'F'>;
template class Port<'G'>;
template class Port<'H'>;
template class Port<'J'>;
template class Port<'K'>;
template class Port<'L'>;
