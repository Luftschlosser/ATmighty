/*!
 * This headerfile is µC-independent. It declares class AbstractHardwareBase, which is the base class for all abstract hardware.
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_ABSTRACTHARDWAREBASE_ABSTRACTHARDWAREBASE_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_ABSTRACTHARDWAREBASE_ABSTRACTHARDWAREBASE_H_


#include <stdbool.h>
#include <stdint.h>

/// This class is a common baseclass for all abstract hardware and manages the exclusive access of each hardware.
class AbstractHardwareBase
{
	private:
		/// 1 Byte overhead stores owner (0 if free) - Positive range is for user, Negative range is for ATmighty
		volatile int8_t owner;

	protected:
		/// Constructor (initializes this abstract hardware as "free")
		inline AbstractHardwareBase();

		/// Destructor
		virtual ~AbstractHardwareBase() {}

		/// Checks if this abstract hardware is in use
		inline bool isFree() {return (owner == 0);}

		/*!
		 * Tries to allocate this abstract hardware
		 * If this method gets overridden in derived types, this base-class implementation must still be called!
		 * \param ownerId The owner-id of the caller who wants to allocate this abstract hardware
		 * \returns True on sucess, false if this abstract hardware is already in use
		 */
		virtual bool init(int8_t ownerId);

		virtual void exit();

		/// Returns the current owner id
		inline int8_t getOwner() {return owner;}
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_ABSTRACTHARDWAREBASE_ABSTRACTHARDWAREBASE_H_ */
