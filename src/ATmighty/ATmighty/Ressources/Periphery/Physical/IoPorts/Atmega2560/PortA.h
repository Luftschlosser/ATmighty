/*!
 * This headerfile is µC-dependent, it should only be included indirectly via "IoPorts.h".
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_IOPORTS_ATMEGA2560_PORTA_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_IOPORTS_ATMEGA2560_PORTA_H_


#include <stdint.h>
#include <avr/io.h>
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareBase/PhysicalHardwareBase.h"
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"

/// This class provides access to the PortA-periphery of the atmega2560 µC.
class PortA : private PhysicalHardwareBase
{
	friend class PhysicalHardwareManager;

	private:
		///Default Constructor
		inline PortA() : PhysicalHardwareBase() {}

		///Default Destructor
		inline ~PortA() {}

	public:
		///Sets the value of the PORTA register
		inline void setPORTA(uint8_t value) {PORTA = value;}
		///Sets the value of the DDRA register
		inline void setDDRA(uint8_t value) {DDRA = value;}
		///Sets the value of the PINA register
		inline void setPINA(uint8_t value) {PINA = value;}

		///Returns the current value of the PORTA register
		inline uint8_t getPORTA() {return PORTA;}
		///Returns the current value of the DDRA register
		inline uint8_t getDDRA() {return DDRA;}
		///Returns the current value of the PINA register
		inline uint8_t getPINA() {return PINA;}
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_IOPORTS_ATMEGA2560_PORTA_H_ */
