/*!
 * This headerfile is µC-dependent, it should only be included indirectly via "IoPorts.h".
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_IOPORTS_ATMEGA2560_PORTC_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_IOPORTS_ATMEGA2560_PORTC_H_


#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareBase/PhysicalHardwareBase.h"
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"

/// This class provides access to the PortC-periphery of the atmega2560 µC.
class PortC : private PhysicalHardwareBase
{
	friend PortC* PhysicalHardwareManager::Alloc<PortC>(int8_t id);
	friend int8_t PhysicalHardwareManager::GetOwner<PortC>();
	friend void PhysicalHardwareManager::Free<PortC>(PortC **hardware);

	private:
		/// Default Constructor
		inline PortC() : PhysicalHardwareBase() {}

		/// returns a singleton-instance
		inline static PortC& GetInstance()
		{
			static PortC instance;
			return instance;
		}

		/// returns a string-representation of this type stored in flash
		inline static PGM_P GetHardwareStringRepresentation()
		{
			static const char me[] PROGMEM = "PortC";
			return me;
		}

	public:
		///Sets the value of the PORTC register
		inline void setPORTC(uint8_t value) {PORTC = value;}
		///Sets the value of the DDRC register
		inline void setDDRC(uint8_t value) {DDRC = value;}
		///Sets the value of the PINC register
		inline void setPINC(uint8_t value) {PINC = value;}

		///Returns the current value of the PORTC register
		inline uint8_t getPORTC() {return PORTC;}
		///Returns the current value of the DDRC register
		inline uint8_t getDDRC() {return DDRC;}
		///Returns the current value of the PINC register
		inline uint8_t getPINC() {return PINC;}
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_IOPORTS_ATMEGA2560_PORTC_H_ */
