/*!
 * This headerfile is µC-dependent, it should only be included indirectly via "Timer.h".
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_TIMER_ATMEGA2560_TIMER2_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_TIMER_ATMEGA2560_TIMER2_H_


#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareBase/PhysicalHardwareBase.h"
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"


/// This class provides access to the Timer2-periphery of the atmega2560 µC.
class Timer2 final : private PhysicalHardwareBase
{
	friend Timer2* PhysicalHardwareManager::Alloc<Timer2>(int8_t id);
	friend int8_t PhysicalHardwareManager::GetOwner<Timer2>();
	friend void PhysicalHardwareManager::Free<Timer2>(Timer2 **hardware);

	private:
		/// Default Constructor
		inline Timer2() : PhysicalHardwareBase() {}

		/// returns a singleton-instance
		inline static Timer2& GetInstance()
		{
			static Timer2 instance;
			return instance;
		}

		/// returns a string-representation of this type stored in flash
		inline static PGM_P GetHardwareStringRepresentation()
		{
			static const char me[] PROGMEM = "Timer2";
			return me;
		}

	public:
		///Sets the value of the TCCR2A register
		inline void setTCCR2A(uint8_t value) {TCCR2A = value;}
		///Sets the value of the TCCR2B register
		inline void setTCCR2B(uint8_t value) {TCCR2B = value;}
		///Sets the value of the TCNT2 register
		inline void setTCNT2(uint8_t value) {TCNT2 = value;}
		///Sets the value of the OCR2A register
		inline void setOCR2A(uint8_t value) {OCR2A = value;}
		///Sets the value of the OCR2B register
		inline void setOCR2B(uint8_t value) {OCR2B = value;}
		///Sets the value of the ASSR register
		inline void setASSR(uint8_t value) {ASSR = value;}
 		///Sets the value of the TIMSK2 register
		inline void setTIMSK2(uint8_t value) {TIMSK2 = value;}
		///Sets the value of the TIFR2 register
		inline void setTIFR2(uint8_t value) {TIFR2 = value;}

		///Returns the current value of the TCCR2A register
		inline uint8_t getTCCR2A() {return TCCR2A;}
		///Returns the current value of the TCCR2B register
		inline uint8_t getTCCR2B() {return TCCR2B;}
		///Returns the current value of the TCNT2 register
		inline uint8_t getTCNT2() {return TCNT2;}
		///Returns the current value of the OCR2A register
		inline uint8_t getOCR2A() {return OCR2A;}
		///Returns the current value of the OCR2B register
		inline uint8_t getOCR2B() {return OCR2B;}
		///Returns the current value of the ASSR register
		inline uint8_t getASSR() {return ASSR;}
		///Returns the current value of the TIMSK2 register
		inline uint8_t getTIMSK2() {return TIMSK2;}
		///Returns the current value of the TIFR2 register
		inline uint8_t getTIFR2() {return TIFR2;}
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_TIMER_ATMEGA2560_TIMER2_H_ */
