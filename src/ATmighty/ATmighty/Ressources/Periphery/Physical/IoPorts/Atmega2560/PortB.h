/*!
 * This headerfile is µC-dependent, it should only be included indirectly via "IoPorts.h".
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_IOPORTS_ATMEGA2560_PORTB_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_IOPORTS_ATMEGA2560_PORTB_H_


#include <stdint.h>
#include <avr/io.h>
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareBase/PhysicalHardwareBase.h"
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"

/// This class provides access to the PortB-periphery of the atmega2560 µC.
class PortB : private PhysicalHardwareBase
{
	friend PortB* PhysicalHardwareManager::Alloc<PortB>(int8_t id);
	friend int8_t PhysicalHardwareManager::GetOwner<PortB>();
	friend void PhysicalHardwareManager::Free<PortB>(PortB **hardware);

	private:
		///Default Constructor
		inline PortB() : PhysicalHardwareBase() {}

		inline static PortB& GetInstance()
		{
			static PortB instance;
			return instance;
		}

	public:
		///Sets the value of the PORTB register
		inline void setPORTB(uint8_t value) {PORTB = value;}
		///Sets the value of the DDRB register
		inline void setDDRB(uint8_t value) {DDRB = value;}
		///Sets the value of the PINB register
		inline void setPINB(uint8_t value) {PINB = value;}

		///Returns the current value of the PORTB register
		inline uint8_t getPORTB() {return PORTB;}
		///Returns the current value of the DDRB register
		inline uint8_t getDDRB() {return DDRB;}
		///Returns the current value of the PINB register
		inline uint8_t getPINB() {return PINB;}
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_IOPORTS_ATMEGA2560_PORTB_H_ */
