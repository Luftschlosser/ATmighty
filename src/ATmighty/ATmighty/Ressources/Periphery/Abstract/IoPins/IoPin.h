/*
 * This headerfile represents a common interface for all 1bit IoPins. (abstract and virtual)
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPINS_IOPIN_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPINS_IOPIN_H_


#include <stdint.h>
#include <avr/pgmspace.h>


///This class represents a pure virtual common interface for all 1bit IoPins
class IoPin
{
	friend class AbstractHardwareManager;

	public:
		/// Enum type-declaration for Input/Output-Direction Values
		enum DataDirection : uint8_t
		{
			Input = 0,
			Output = 1
		};

		/// Virtual Destructor
		virtual ~IoPin() {}

		/// Sets the data-direction of this pin. ('0' = in, '>0' = out)
		virtual void setDirection(DataDirection direction) = 0;

		/*!
		 * Sets the output logic-level of this pin, when configured as output-pin. (true = high, false = low)
		 * Enables/disables the pullup-resistor of this pin, when configured as input-pin (true = enable, false = disable)
		 */
		virtual void set(bool value) = 0;

		/// toggles the output of this pin independently of the data-direction
		virtual void toggle() = 0;

		/// returns the current data-direction of this pin
		virtual DataDirection getDirection() = 0;

		/// returns the current value of this pin, independently of the data-direction. (true = high, false = low)
		virtual bool read() = 0;


	private:
		/// returns a string-representation of this type stored in flash
		inline PGM_P getHardwareStringRepresentation()
		{
			static const char me[] PROGMEM = "IoPin";
			return me;
		}
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPINS_IOPIN_H_ */
