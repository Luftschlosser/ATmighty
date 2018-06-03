/*
 * This headerfile represents a common interface for all 8bit IoPorts. (abstract and virtual)
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPORTS_IOPORT_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPORTS_IOPORT_H_


#include <stdint.h>

///This class represents a pure virtual common interface for all 8bit IoPorts
class IoPort
{
	public:
		/// Virtual Destructor
		virtual ~IoPort() {}

		/// Sets the data-direction-bits of this port. ('1' = out, '0' = in)
		virtual void setDataDirectionMask(uint8_t mask) = 0;

		/*!
		 * Sets the data-register of this port.
		 * Pins which are configured as output: Sets the output logic-level ('1' = high, '0' = low)
		 * Pins which are configured as input: Sets the pullup-resistors ('1' = enable, '0' = disable)
		 */
		virtual void setData(uint8_t data) = 0;

		/// toggles the pins where mask == '1' independently of the data-direction.
		virtual void applyPinToggleMask(uint8_t mask) = 0;

		/// Reads the data-direction-bits of this port. ('1' = out, '0' = in)
		virtual uint8_t getDataDirections() = 0;

		/// Reads the value of the pins of this port independently of the data-direction.
		virtual uint8_t getPinValues() = 0;
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPORTS_IOPORT_H_ */
