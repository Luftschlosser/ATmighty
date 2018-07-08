/*!
 * This headerfile is µC-dependent, it should only be included indirectly via "Timer.h".
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_TIMER_ATMEGA2560_TIMER5_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_TIMER_ATMEGA2560_TIMER5_H_


#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareBase/PhysicalHardwareBase.h"
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"


/// This class provides access to the Timer5-periphery of the atmega2560 µC.
class Timer5 final : private PhysicalHardwareBase
{
	friend Timer5* PhysicalHardwareManager::Alloc<Timer5>(int8_t id);
	friend int8_t PhysicalHardwareManager::GetOwner<Timer5>();
	friend void PhysicalHardwareManager::Free<Timer5>(Timer5 **hardware);

	private:
		/// Default Constructor
		inline Timer5() : PhysicalHardwareBase() {}

		/// returns a singleton-instance
		inline static Timer5& GetInstance()
		{
			static Timer5 instance;
			return instance;
		}

		/// returns a string-representation of this type stored in flash
		inline static PGM_P GetHardwareStringRepresentation()
		{
			static const char me[] PROGMEM = "Timer5";
			return me;
		}

	public:
		///Sets the value of the TCCR5A register
		inline void setTCCR5A(uint8_t value) {TCCR5A = value;}
		///Sets the value of the TCCR5B register
		inline void setTCCR5B(uint8_t value) {TCCR5B = value;}
		///Sets the value of the TCCR5C register
		inline void setTCCR5C(uint8_t value) {TCCR5C = value;}
		///Sets the value of the TCNT5 register
		inline void setTCNT5(uint16_t value) {TCNT5 = value;}
		///Sets the value of the OCR5A register
		inline void setOCR5A(uint16_t value) {OCR5A = value;}
		///Sets the value of the OCR5B register
		inline void setOCR5B(uint16_t value) {OCR5B = value;}
		///Sets the value of the OCR5C register
		inline void setOCR5C(uint16_t value) {OCR5C = value;}
		///Sets the value of the ICR5 register
		inline void setICR5(uint16_t value) {ICR5 = value;}
		///Sets the value of the TIMSK5 register
		inline void setTIMSK5(uint8_t value) {TIMSK5 = value;}
		///Sets the value of the TIFR5 register
		inline void setTIFR5(uint8_t value) {TIFR5 = value;}

		///Returns the current value of the TCCR5A register
		inline uint8_t getTCCR5A() {return TCCR5A;}
		///Returns the current value of the TCCR5B register
		inline uint8_t getTCCR5B() {return TCCR5B;}
		///Returns the current value of the TCCR5C register
		inline uint8_t getTCCR5C() {return TCCR5C;}
		///Returns the current value of the TCNT5 register
		inline uint16_t getTCNT5() {return TCNT5;}
		///Returns the current value of the OCR5A register
		inline uint16_t getOCR5A() {return OCR5A;}
		///Returns the current value of the OCR5B register
		inline uint16_t getOCR5B() {return OCR5B;}
		///Returns the current value of the OCR5C register
		inline uint16_t getOCR5C() {return OCR5C;}
		///Returns the current value of the ICR5 register
		inline uint16_t getICR5() {return ICR5;}
		///Returns the current value of the TIMSK5 register
		inline uint8_t getTIMSK5() {return TIMSK5;}
		///Returns the current value of the TIFR5 register
		inline uint8_t getTIFR5() {return TIFR5;}
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_TIMER_ATMEGA2560_TIMER5_H_ */
