/*!
 * This headerfile is µC-dependent, it should only be included indirectly via "IoPorts.h".
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_IOPORTS_ATMEGA2560_PORTD_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_IOPORTS_ATMEGA2560_PORTD_H_


#include <stdint.h>
#include <avr/io.h>
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareBase/PhysicalHardwareBase.h"
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"

/// This class provides access to the PortD-periphery of the atmega2560 µC.
class PortD : private PhysicalHardwareBase
{
	friend PortD* PhysicalHardwareManager::Alloc<PortD>(int8_t id);
	friend int8_t PhysicalHardwareManager::GetOwner<PortD>();
	friend void PhysicalHardwareManager::Free<PortD>(PortD **hardware);

	private:
		///Default Constructor
		inline PortD() : PhysicalHardwareBase() {}

		inline static PortD& GetInstance()
		{
			static PortD instance;
			return instance;
		}

	public:
		///Sets the value of the PortD register
		inline void setPORTD(uint8_t value) {PORTD = value;}
		///Sets the value of the DDRD register
		inline void setDDRD(uint8_t value) {DDRD = value;}
		///Sets the value of the PIND register
		inline void setPIND(uint8_t value) {PIND = value;}

		///Returns the current value of the PortD register
		inline uint8_t getPORTD() {return PORTD;}
		///Returns the current value of the DDRD register
		inline uint8_t getDDRD() {return DDRD;}
		///Returns the current value of the PIND register
		inline uint8_t getPIND() {return PIND;}
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_IOPORTS_ATMEGA2560_PortD_H_ */
