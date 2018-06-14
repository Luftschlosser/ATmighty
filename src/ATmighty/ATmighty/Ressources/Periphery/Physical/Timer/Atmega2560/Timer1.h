/*!
 * This headerfile is µC-dependent, it should only be included indirectly via "Timer.h".
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_TIMER_ATMEGA2560_TIMER1_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_TIMER_ATMEGA2560_TIMER1_H_


#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareBase/PhysicalHardwareBase.h"
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"


/// This class provides access to the Timer1-periphery of the atmega2560 µC.
class Timer1 : private PhysicalHardwareBase
{
	friend Timer1* PhysicalHardwareManager::Alloc<Timer1>(int8_t id);
	friend int8_t PhysicalHardwareManager::GetOwner<Timer1>();
	friend void PhysicalHardwareManager::Free<Timer1>(Timer1 **hardware);

	private:
		/// Default Constructor
		inline Timer1() : PhysicalHardwareBase() {}

		/// returns a singleton-instance
		inline static Timer1& GetInstance()
		{
			static Timer1 instance;
			return instance;
		}

		/// returns a string-representation of this type stored in flash
		inline static PGM_P GetHardwareStringRepresentation()
		{
			static const char me[] PROGMEM = "Timer1";
			return me;
		}

	public:
		///Sets the value of the TCCR1A register
		inline void setTCCR1A(uint8_t value) {TCCR1A = value;}
		///Sets the value of the TCCR1B register
		inline void setTCCR1B(uint8_t value) {TCCR1B = value;}
		///Sets the value of the TCCR1C register
		inline void setTCCR1C(uint8_t value) {TCCR1C = value;}
		///Sets the value of the TCNT1 register
		inline void setTCNT1(uint16_t value) {TCNT1 = value;}
		///Sets the value of the OCR1A register
		inline void setOCR1A(uint16_t value) {OCR1A = value;}
		///Sets the value of the OCR1B register
		inline void setOCR1B(uint16_t value) {OCR1B = value;}
		///Sets the value of the OCR1C register
		inline void setOCR1C(uint16_t value) {OCR1C = value;}
		///Sets the value of the ICR1 register
		inline void setICR1(uint16_t value) {ICR1 = value;}
		///Sets the value of the TIMSK1 register
		inline void setTIMSK1(uint8_t value) {TIMSK1 = value;}
		///Sets the value of the TIFR1 register
		inline void setTIFR1(uint8_t value) {TIFR1 = value;}

		///Returns the current value of the TCCR1A register
		inline uint8_t getTCCR1A() {return TCCR1A;}
		///Returns the current value of the TCCR1B register
		inline uint8_t getTCCR1B() {return TCCR1B;}
		///Returns the current value of the TCCR1C register
		inline uint8_t getTCCR1C() {return TCCR1C;}
		///Returns the current value of the TCNT1 register
		inline uint16_t getTCNT1() {return TCNT1;}
		///Returns the current value of the OCR1A register
		inline uint16_t getOCR1A() {return OCR1A;}
		///Returns the current value of the OCR1B register
		inline uint16_t getOCR1B() {return OCR1B;}
		///Returns the current value of the OCR1C register
		inline uint16_t getOCR1C() {return OCR1C;}
		///Returns the current value of the ICR1 register
		inline uint16_t getICR1() {return ICR1;}
		///Returns the current value of the TIMSK1 register
		inline uint8_t getTIMSK1() {return TIMSK1;}
		///Returns the current value of the TIFR1 register
		inline uint8_t getTIFR1() {return TIFR1;}
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_TIMER_ATMEGA2560_TIMER1_H_ */
