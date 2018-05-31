/*
 * This headerfile represents a common interface for all 8bit IoPorts.
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPORTS_IOPORT_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPORTS_IOPORT_H_


#include <stdint.h>
#include "ATmighty/Ressources/Periphery/Abstract/AbstractHardwareBase/AbstractHardwareBase.h"
#include "ATmighty/Ressources/Periphery/Abstract/AbstractHardwareManager.h"

///This class represents a common interface for all 8bit IoPorts
class IoPort : public AbstractHardwareBase
{
	friend class AbstractHardwareManager;

	protected:
		/// Default Constructor - Can only be called by friends
		inline IoPort() {}

		/// Virtual Destructor
		virtual ~IoPort() {}

		/*!
		 * Tries to allocate this abstract hardware, initializes physical hardware dependencies.
		 * This routine can only be called by friends.
		 * \param ownerId The owner-id of the caller who wants to allocate this abstract hardware
		 * \returns 0 on success, >0 if this abstract hardware is already in use or <0 if the physical hardware dependencies could not be resolved.
		 */
		virtual int8_t init(int8_t ownerId) = 0;

		/*!
		 * Frees this abstract hardware and the allocated physical hardware dependencies.
		 * This routine can only be called by friends.
		 */
		virtual void exit() = 0;

	public:
		/// Sets the data-direction-bits of this port. ('1' = out, '0' = in)
		virtual void setDataDirectionMask(uint8_t mask) = 0;

		/*!
		 * Sets the data-register of this port.
		 * Pins which are configured as output: Sets the output logic-level ('1' = high, '0' = low)
		 * Pins which are configured as input: Sets the pullup-resistors ('1' = enable, '0' = disable)
		 */
		virtual void setData(uint8_t data) = 0;

		/// toggles the Pins where mask == '1' independently of the data-direction.
		virtual void applyPinToggleMask(uint8_t mask) = 0;

		/// Reads the data-direction-bits of this port. ('1' = out, '0' = in)
		virtual uint8_t getDataDirections() = 0;

		/// Reads the value of the pins of this port independently of the data-direction.
		virtual uint8_t getPinValues() = 0;
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPORTS_IOPORT_H_ */
