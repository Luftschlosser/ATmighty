/*
 * This headerfile represents a common interface for all abstract 1bit IoPins.
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPINS_ABSTRACTIOPIN_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPINS_ABSTRACTIOPIN_H_


#include <stdint.h>
#include "ATmighty/Ressources/Periphery/Abstract/IoPins/IoPin.h"


///This class represents a common interface for all abstract 1bit IoPins
class AbstractIoPin : public IoPin
{
	friend class AbstractHardwareManager;

	protected:
		/// Default Constructor - Can only be called by friends
		inline AbstractIoPin() {}

		/// Virtual Destructor
		virtual ~AbstractIoPin() {}

		/*!
		 * Frees this abstract hardware and the allocated hardware dependencies.
		 * This routine can only be called by friends.
		 */
		virtual void exit() = 0;

		/// Returns the current owner id
		virtual int8_t getOwner() = 0;

	public:
		/// Returns the corresponding port-character associated with the specific port used by this IoPin abstraction \returns ('A' - 'L')
		virtual char getPinPort() = 0;

		/// Returns the corresponding pin-number associated with this specific pin within its IoPort (as printable character) \returns ('0' - '7')
		virtual char getPinNumber() = 0;
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPINS_ABSTRACTIOPIN_H_ */
