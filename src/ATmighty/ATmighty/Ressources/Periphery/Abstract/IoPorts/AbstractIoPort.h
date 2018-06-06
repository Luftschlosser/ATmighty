/*
 * This headerfile represents a common interface for all abstract 8bit IoPorts.
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPORTS_ABSTRACTIOPORT_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPORTS_ABSTRACTIOPORT_H_


#include <stdint.h>
#include "ATmighty/Ressources/Periphery/Abstract/IoPorts/IoPort.h"


///This class represents a common interface for all abstract 8bit IoPorts
class AbstractIoPort : public IoPort
{
	friend class AbstractHardwareManager;

	protected:
		/// Default Constructor - Can only be called by friends
		inline AbstractIoPort() {}

		/// Virtual Destructor
		virtual ~AbstractIoPort() {}

		/*!
		 * Frees this abstract hardware and the allocated physical hardware dependencies.
		 * This routine can only be called by friends.
		 */
		virtual void exit() = 0;

		/// Returns the current owner id
		virtual int8_t getOwner() = 0;

	public:
		/// Returns the corresponding letter (upper-case) associated with the specific physical IoPort used by this AbstractIoPort \returns ('A'-'L')
		virtual char getCharCode() = 0;
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPORTS_ABSTRACTIOPORT_H_ */
