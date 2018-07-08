/*!
 * This headerfile is µC-dependent, it should only be included indirectly via "Timer.h".
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_TIMER_ATMEGA2560_TIMER4_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_TIMER_ATMEGA2560_TIMER4_H_


#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareBase/PhysicalHardwareBase.h"
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"


/// This class provides access to the Timer4-periphery of the atmega2560 µC.
class Timer4 final : private PhysicalHardwareBase
{
	friend Timer4* PhysicalHardwareManager::Alloc<Timer4>(int8_t id);
	friend int8_t PhysicalHardwareManager::GetOwner<Timer4>();
	friend void PhysicalHardwareManager::Free<Timer4>(Timer4 **hardware);

	private:
		/// Default Constructor
		inline Timer4() : PhysicalHardwareBase() {}

		/// returns a singleton-instance
		inline static Timer4& GetInstance()
		{
			static Timer4 instance;
			return instance;
		}

		/// returns a string-representation of this type stored in flash
		inline static PGM_P GetHardwareStringRepresentation()
		{
			static const char me[] PROGMEM = "Timer4";
			return me;
		}

	public:
		///Sets the value of the TCCR4A register
		inline void setTCCR4A(uint8_t value) {TCCR4A = value;}
		///Sets the value of the TCCR4B register
		inline void setTCCR4B(uint8_t value) {TCCR4B = value;}
		///Sets the value of the TCCR4C register
		inline void setTCCR4C(uint8_t value) {TCCR4C = value;}
		///Sets the value of the TCNT4 register
		inline void setTCNT4(uint16_t value) {TCNT4 = value;}
		///Sets the value of the OCR4A register
		inline void setOCR4A(uint16_t value) {OCR4A = value;}
		///Sets the value of the OCR4B register
		inline void setOCR4B(uint16_t value) {OCR4B = value;}
		///Sets the value of the OCR4C register
		inline void setOCR4C(uint16_t value) {OCR4C = value;}
		///Sets the value of the ICR4 register
		inline void setICR4(uint16_t value) {ICR4 = value;}
		///Sets the value of the TIMSK4 register
		inline void setTIMSK4(uint8_t value) {TIMSK4 = value;}
		///Sets the value of the TIFR4 register
		inline void setTIFR4(uint8_t value) {TIFR4 = value;}

		///Returns the current value of the TCCR4A register
		inline uint8_t getTCCR4A() {return TCCR4A;}
		///Returns the current value of the TCCR4B register
		inline uint8_t getTCCR4B() {return TCCR4B;}
		///Returns the current value of the TCCR4C register
		inline uint8_t getTCCR4C() {return TCCR4C;}
		///Returns the current value of the TCNT4 register
		inline uint16_t getTCNT4() {return TCNT4;}
		///Returns the current value of the OCR4A register
		inline uint16_t getOCR4A() {return OCR4A;}
		///Returns the current value of the OCR4B register
		inline uint16_t getOCR4B() {return OCR4B;}
		///Returns the current value of the OCR4C register
		inline uint16_t getOCR4C() {return OCR4C;}
		///Returns the current value of the ICR4 register
		inline uint16_t getICR4() {return ICR4;}
		///Returns the current value of the TIMSK4 register
		inline uint8_t getTIMSK4() {return TIMSK4;}
		///Returns the current value of the TIFR4 register
		inline uint8_t getTIFR4() {return TIFR4;}
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_TIMER_ATMEGA2560_TIMER4_H_ */
