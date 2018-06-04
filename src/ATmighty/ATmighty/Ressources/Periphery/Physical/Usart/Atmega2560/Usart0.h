/*!
 * This headerfile is µC-dependent, it should only be included indirectly via "Usart.h".
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_USART_ATMEGA2560_USART0_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_USART_ATMEGA2560_USART0_H_


#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareBase/PhysicalHardwareBase.h"
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"


/// This class provides access to the Usart0-periphery of the atmega2560 µC.
class Usart0 : private PhysicalHardwareBase
{
	friend Usart0* PhysicalHardwareManager::Alloc<Usart0>(int8_t id);
	friend int8_t PhysicalHardwareManager::GetOwner<Usart0>();
	friend void PhysicalHardwareManager::Free<Usart0>(Usart0 **hardware);

	private:
		/// Default Constructor
		inline Usart0() : PhysicalHardwareBase() {}

		/// returns a singleton-instance
		inline static Usart0& GetInstance()
		{
			static Usart0 instance;
			return instance;
		}

		/// returns a string-representation of this type stored in flash
		inline static PGM_P GetHardwareStringRepresentation()
		{
			static const char me[] PROGMEM = "Usart0";
			return me;
		}

	public:
		///Sets the value of the UDR0 register
		inline void setUDR0(uint8_t value) {UDR0 = value;}
		///Sets the value of the UCSR0A register
		inline void setUCSR0A(uint8_t value) {UCSR0A = value;}
		///Sets the value of the UCSR0B register
		inline void setUCSR0B(uint8_t value) {UCSR0B = value;}
		///Sets the value of the UCSR0C register
		inline void setUCSR0C(uint8_t value) {UCSR0C = value;}
		///Sets the value of the UBRR0L register
		inline void setUBRR0L(uint8_t value) {UBRR0L = value;}
		///Sets the value of the UBRR0H register
		inline void setUBRR0H(uint8_t value) {UBRR0H = value;}

		///Returns the current value of the UDR0 register
		inline uint8_t getUDR0() {return UDR0;}
		///Returns the current value of the UCSR0A register
		inline uint8_t getUCSR0A() {return UCSR0A;}
		///Returns the current value of the UCSR0B register
		inline uint8_t getUCSR0B() {return UCSR0B;}
		///Returns the current value of the UCSR0C register
		inline uint8_t getUCSR0C() {return UCSR0C;}
		///Returns the current value of the UBRR0L register
		inline uint8_t getUBRR0L() {return UBRR0L;}
		///Returns the current value of the UBRR0H register
		inline uint8_t getUBRR0H() {return UBRR0H;}
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_USART_ATMEGA2560_USART0_H_ */
