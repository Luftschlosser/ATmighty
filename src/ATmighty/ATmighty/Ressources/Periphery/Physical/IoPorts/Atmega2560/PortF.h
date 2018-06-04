/*!
 * This headerfile is µC-dependent, it should only be included indirectly via "IoPorts.h".
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_IOPORTS_ATMEGA2560_PORTF_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_IOPORTS_ATMEGA2560_PORTF_H_


#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareBase/PhysicalHardwareBase.h"
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"

/// This class provides access to the PortF-periphery of the atmega2560 µC.
class PortF : private PhysicalHardwareBase
{
	friend PortF* PhysicalHardwareManager::Alloc<PortF>(int8_t id);
	friend int8_t PhysicalHardwareManager::GetOwner<PortF>();
	friend void PhysicalHardwareManager::Free<PortF>(PortF **hardware);

	private:
		/// Default Constructor
		inline PortF() : PhysicalHardwareBase() {}

		/// returns a singleton-instance
		inline static PortF& GetInstance()
		{
			static PortF instance;
			return instance;
		}

		/// returns a string-representation of this type stored in flash
		inline static PGM_P GetHardwareStringRepresentation()
		{
			static const char me[] PROGMEM = "PortF";
			return me;
		}

	public:
		///Sets the value of the PORTF register
		inline void setPORTF(uint8_t value) {PORTF = value;}
		///Sets the value of the DDRF register
		inline void setDDRF(uint8_t value) {DDRF = value;}
		///Sets the value of the PINF register
		inline void setPINF(uint8_t value) {PINF = value;}

		///Returns the current value of the PORTF register
		inline uint8_t getPORTF() {return PORTF;}
		///Returns the current value of the DDRF register
		inline uint8_t getDDRF() {return DDRF;}
		///Returns the current value of the PINF register
		inline uint8_t getPINF() {return PINF;}
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_IOPORTS_ATMEGA2560_PORTF_H_ */
