/*!
 * This headerfile is µC-dependent, it should only be included indirectly via "Timer.h".
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_TIMER_ATMEGA2560_TIMER3_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_TIMER_ATMEGA2560_TIMER3_H_


#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareBase/PhysicalHardwareBase.h"
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"


/// This class provides access to the Timer3-periphery of the atmega2560 µC.
class Timer3 final : private PhysicalHardwareBase
{
	friend Timer3* PhysicalHardwareManager::Alloc<Timer3>(int8_t id);
	friend int8_t PhysicalHardwareManager::GetOwner<Timer3>();
	friend void PhysicalHardwareManager::Free<Timer3>(Timer3 **hardware);

	private:
		/// Default Constructor
		inline Timer3() : PhysicalHardwareBase() {}

		/// returns a singleton-instance
		inline static Timer3& GetInstance()
		{
			static Timer3 instance;
			return instance;
		}

		/// returns a string-representation of this type stored in flash
		inline static PGM_P GetHardwareStringRepresentation()
		{
			static const char me[] PROGMEM = "Timer3";
			return me;
		}

	public:
		///Sets the value of the TCCR3A register
		inline void setTCCR3A(uint8_t value) {TCCR3A = value;}
		///Sets the value of the TCCR3B register
		inline void setTCCR3B(uint8_t value) {TCCR3B = value;}
		///Sets the value of the TCCR3C register
		inline void setTCCR3C(uint8_t value) {TCCR3C = value;}
		///Sets the value of the TCNT3 register
		inline void setTCNT3(uint16_t value) {TCNT3 = value;}
		///Sets the value of the OCR3A register
		inline void setOCR3A(uint16_t value) {OCR3A = value;}
		///Sets the value of the OCR3B register
		inline void setOCR3B(uint16_t value) {OCR3B = value;}
		///Sets the value of the OCR3C register
		inline void setOCR3C(uint16_t value) {OCR3C = value;}
		///Sets the value of the ICR3 register
		inline void setICR3(uint16_t value) {ICR3 = value;}
		///Sets the value of the TIMSK3 register
		inline void setTIMSK3(uint8_t value) {TIMSK3 = value;}
		///Sets the value of the TIFR3 register
		inline void setTIFR3(uint8_t value) {TIFR3 = value;}

		///Returns the current value of the TCCR3A register
		inline uint8_t getTCCR3A() {return TCCR3A;}
		///Returns the current value of the TCCR3B register
		inline uint8_t getTCCR3B() {return TCCR3B;}
		///Returns the current value of the TCCR3C register
		inline uint8_t getTCCR3C() {return TCCR3C;}
		///Returns the current value of the TCNT3 register
		inline uint16_t getTCNT3() {return TCNT3;}
		///Returns the current value of the OCR3A register
		inline uint16_t getOCR3A() {return OCR3A;}
		///Returns the current value of the OCR3B register
		inline uint16_t getOCR3B() {return OCR3B;}
		///Returns the current value of the OCR3C register
		inline uint16_t getOCR3C() {return OCR3C;}
		///Returns the current value of the ICR3 register
		inline uint16_t getICR3() {return ICR3;}
		///Returns the current value of the TIMSK3 register
		inline uint8_t getTIMSK3() {return TIMSK3;}
		///Returns the current value of the TIFR3 register
		inline uint8_t getTIFR3() {return TIFR3;}
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_TIMER_ATMEGA2560_TIMER3_H_ */
