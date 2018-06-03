/*
 * This headerfile represents a common interface for all abstract 8bit IoPorts.
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPORTS_ABSTRACTIOPORT_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPORTS_ABSTRACTIOPORT_H_


#include <stdint.h>
#include "ATmighty/Ressources/Periphery/Abstract/IoPorts/IoPort.h"
#include "ATmighty/Ressources/Periphery/Abstract/AbstractHardwareManager.h"

///This class represents a common interface for all abstract 8bit IoPorts
class AbstractIoPort : public IoPort
{
	friend class AbstractHardwareManager;

	private:
		/// 1 Byte overhead stores owner (0 if free) - Positive range is for user, Negative range is for ATmighty
		volatile int8_t owner;

	protected:
		/// Default Constructor - Can only be called by friends
		inline AbstractIoPort() {}

		/// Virtual Destructor
		virtual ~AbstractIoPort() {}

		/*!
		 * Tries to allocate this abstract hardware, initializes physical hardware dependencies.
		 * When this method gets overridden in derived types, this base-class implementation must still be called!
		 * This routine can only be called by friends.
		 * \param ownerId The owner-id of the caller who wants to allocate this abstract hardware
		 * \returns 0 on success, >0 if this abstract hardware is already in use or <0 if the physical hardware dependencies could not be resolved.
		 */
		virtual inline int8_t init(int8_t ownerId) {
			if (owner == 0)
			{
				owner = ownerId;
				return 0;
			}
			else
			{
				return 1;
			}
		}

		/*!
		 * Frees this abstract hardware and the allocated physical hardware dependencies.
		 * When this method gets overridden in derived types, this base-class implementation must still be called!
		 * This routine can only be called by friends.
		 */
		virtual inline void exit() {
			owner = 0;
		}

		/// Checks if this abstract hardware is in use
		inline bool isFree() {
			return (owner == 0);
		}

		/// Returns the current owner id
		inline int8_t getOwner() {
			return owner;
		}

	public:
		/// Returns the corresponding letter (upper-case) associated with the specific physical IoPort used by this AbstractIoPort ('A'-'L')
		virtual char getCharCode() = 0;
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPORTS_ABSTRACTIOPORT_H_ */
