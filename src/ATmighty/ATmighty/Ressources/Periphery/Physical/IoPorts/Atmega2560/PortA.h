/*!
 * This headerfile is µC-dependent, it should only be included indirectly via "IoPorts.h".
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_IOPORTS_ATMEGA2560_PORTA_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_IOPORTS_ATMEGA2560_PORTA_H_


#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareBase/PhysicalHardwareBase.h"
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"

/// This class provides access to the PortA-periphery of the atmega2560 µC.
class PortA : private PhysicalHardwareBase
{
	friend PortA* PhysicalHardwareManager::Alloc<PortA>(int8_t id);
	friend int8_t PhysicalHardwareManager::GetOwner<PortA>();
	friend void PhysicalHardwareManager::Free<PortA>(PortA **hardware);

	private:
		/// Default Constructor
		inline PortA() : PhysicalHardwareBase() {}

		/// returns a singleton-instance
		inline static PortA& GetInstance()
		{
			static PortA instance;
			return instance;
		}

		/// returns a string-representation of this type stored in flash
		inline static PGM_P GetHardwareStringRepresentation()
		{
			static const char me[] PROGMEM = "PortA";
			return me;
		}

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
