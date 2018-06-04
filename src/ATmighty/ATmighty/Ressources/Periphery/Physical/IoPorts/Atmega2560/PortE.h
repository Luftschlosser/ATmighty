/*!
 * This headerfile is µC-dependent, it should only be included indirectly via "IoPorts.h".
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_IOPORTS_ATMEGA2560_PORTE_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_IOPORTS_ATMEGA2560_PORTE_H_


#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareBase/PhysicalHardwareBase.h"
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"

/// This class provides access to the PortE-periphery of the atmega2560 µC.
class PortE : private PhysicalHardwareBase
{
	friend PortE* PhysicalHardwareManager::Alloc<PortE>(int8_t id);
	friend int8_t PhysicalHardwareManager::GetOwner<PortE>();
	friend void PhysicalHardwareManager::Free<PortE>(PortE **hardware);

	private:
		/// Default Constructor
		inline PortE() : PhysicalHardwareBase() {}

		/// returns a singleton-instance
		inline static PortE& GetInstance()
		{
			static PortE instance;
			return instance;
		}

		/// returns a string-representation of this type stored in flash
		inline static PGM_P GetHardwareStringRepresentation()
		{
			static const char me[] PROGMEM = "PortE";
			return me;
		}

	public:
		///Sets the value of the PORTE register
		inline void setPORTE(uint8_t value) {PORTE = value;}
		///Sets the value of the DDRE register
		inline void setDDRE(uint8_t value) {DDRE = value;}
		///Sets the value of the PINE register
		inline void setPINE(uint8_t value) {PINE = value;}

		///Returns the current value of the PORTE register
		inline uint8_t getPORTE() {return PORTE;}
		///Returns the current value of the DDRE register
		inline uint8_t getDDRE() {return DDRE;}
		///Returns the current value of the PINE register
		inline uint8_t getPINE() {return PINE;}
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_IOPORTS_ATMEGA2560_PORTE_H_ */
