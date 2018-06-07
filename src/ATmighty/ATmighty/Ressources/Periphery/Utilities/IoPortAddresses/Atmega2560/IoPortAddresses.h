/*!
 * This headerfile is µC-dependent, it should only be included indirectly via "IoPortAddresses.h".
 * This headerfile defines functions to validate a port-character for this µC and to parse a port-character into an IO-address
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_UTILITIES_IOPORTADDRESSES_ATMEGA2560_IOPORTADDRESSES_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_UTILITIES_IOPORTADDRESSES_ATMEGA2560_IOPORTADDRESSES_H_


#include <stdint.h>
#include <avr/io.h>


///This namespace defines functions to validate a port-character for this µC and to parse a port-character into an IO-address
namespace IoPortAddresses
{
	/*!
	 * Tests if the given character is a valid port-character for this µC
	 * \param portChar the character to validate (uppercase, 'A'-'Z')
	 * \returns true if this character can be parsed into a valid IoPort-address for this µC, false if not.
	 */
	inline bool IsValidPortChar(char portChar)
	{
		return !((portChar > 'L') || (portChar < 'A') || (portChar == 'I'));
	}

	/*!
	 * Calculates the address of the IoPort specified by a character. The address will point to the "PINX"-register, "DDRX" and "PORTX" will have an fixed offset of +1/+2.
	 * \param portChar the character which specifies the desired IoPort X
	 * \returns the address of the "PINX"-register of the specified IoPort X
	 */
	inline uint8_t* CharToAddress(char portChar)
	{
		if (portChar <= 'G')
		{
			return (uint8_t*)(__SFR_OFFSET + (portChar-'A')*3);
		}
		else if (portChar == 'H')
		{
			return (uint8_t*)(0x100 + (portChar-'H'));
		}
		else //portChar >= 'I'
		{
			return (uint8_t*)(0x100 + (portChar-'I')*3);
		}
	}

	/*!
	 * Calculates an upper-case char for a given IoPort-address
	 * \param addr the given IoPort-address
	 * \returns an upper-case character which represents the specified IoPort ('A'-'L')
	 */
	inline char AddressToChar(uint8_t* addr)
	{
		if ((uintptr_t)(addr) >= 0x100)
		{
			char c = (char)(((uint8_t)((uintptr_t)(addr - 0x100)))/3 + 'H');
			if (c >= 'I')
			{
				c++;
			}
			return c;
		}
		else
		{
			return (char)(((uint8_t)((uintptr_t)addr) - __SFR_OFFSET)/3 + 'A');
		}
	}
}


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_UTILITIES_IOPORTADDRESSES_ATMEGA2560_IOPORTADDRESSES_H_ */
