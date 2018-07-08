/*!
 * This headerfile is µC-dependent, it should only be included indirectly via "Timer.h".
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_TIMER_ATMEGA2560_TIMER0_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_TIMER_ATMEGA2560_TIMER0_H_


#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareBase/PhysicalHardwareBase.h"
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"


/// This class provides access to the Timer0-periphery of the atmega2560 µC.
class Timer0 final : private PhysicalHardwareBase
{
	friend Timer0* PhysicalHardwareManager::Alloc<Timer0>(int8_t id);
	friend int8_t PhysicalHardwareManager::GetOwner<Timer0>();
	friend void PhysicalHardwareManager::Free<Timer0>(Timer0 **hardware);

	private:
		/// Default Constructor
		inline Timer0() : PhysicalHardwareBase() {}

		/// returns a singleton-instance
		inline static Timer0& GetInstance()
		{
			static Timer0 instance;
			return instance;
		}

		/// returns a string-representation of this type stored in flash
		inline static PGM_P GetHardwareStringRepresentation()
		{
			static const char me[] PROGMEM = "Timer0";
			return me;
		}

	public:
		///Sets the value of the TCCR0A register
		inline void setTCCR0A(uint8_t value) {TCCR0A = value;}
		///Sets the value of the TCCR0B register
		inline void setTCCR0B(uint8_t value) {TCCR0B = value;}
		///Sets the value of the TCNT0 register
		inline void setTCNT0(uint8_t value) {TCNT0 = value;}
		///Sets the value of the OCR0A register
		inline void setOCR0A(uint8_t value) {OCR0A = value;}
		///Sets the value of the OCR0B register
		inline void setOCR0B(uint8_t value) {OCR0B = value;}
		///Sets the value of the TIMSK0 register
		inline void setTIMSK0(uint8_t value) {TIMSK0 = value;}
		///Sets the value of the TIFR0 register
		inline void setTIFR0(uint8_t value) {TIFR0 = value;}

		///Returns the current value of the TCCR0A register
		inline uint8_t getTCCR0A() {return TCCR0A;}
		///Returns the current value of the TCCR0B register
		inline uint8_t getTCCR0B() {return TCCR0B;}
		///Returns the current value of the TCNT0 register
		inline uint8_t getTCNT0() {return TCNT0;}
		///Returns the current value of the OCR0A register
		inline uint8_t getOCR0A() {return OCR0A;}
		///Returns the current value of the OCR0B register
		inline uint8_t getOCR0B() {return OCR0B;}
		///Returns the current value of the TIMSK0 register
		inline uint8_t getTIMSK0() {return TIMSK0;}
		///Returns the current value of the TIFR0 register
		inline uint8_t getTIFR0() {return TIFR0;}
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_TIMER_ATMEGA2560_TIMER0_H_ */
