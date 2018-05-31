/*!
 * This headerfile is µC-independent. It declares class AbstractHardwareBase, which is the base class for all abstract hardware.
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_ABSTRACTHARDWAREBASE_ABSTRACTHARDWAREBASE_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_ABSTRACTHARDWAREBASE_ABSTRACTHARDWAREBASE_H_


#include <stdbool.h>
#include <stdint.h>

/// This class is a common base-class for all abstract hardware and manages the exclusive access of each hardware.
class AbstractHardwareBase
{
	friend class AbstractHardwareManager;

	private:
		/// 1 Byte overhead stores owner (0 if free) - Positive range is for user, Negative range is for ATmighty
		volatile int8_t owner;

	protected:
		/// Constructor (initializes this abstract hardware as "free")
		inline AbstractHardwareBase() : owner(0) {}

		///virtual Destructor
		virtual ~AbstractHardwareBase() {}

		/// Checks if this abstract hardware is in use
		inline bool isFree() {return (owner == 0);}

		/*!
		 * Tries to allocate this abstract hardware, initializes physical hardware dependencies.
		 * If this method gets overridden in derived types, this base-class implementation must still be called!
		 * \param ownerId The owner-id of the caller who wants to allocate this abstract hardware
		 * \returns 0 on success, >0 if this abstract hardware is already in use or <0 if the physical hardware dependencies could not be resolved.
		 */
		virtual int8_t init(int8_t ownerId)
		{
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
		 * If this method gets overridden in derived types, this base-class implementation must still be called!
		 */
		virtual void exit() {owner = 0;}

		/// Returns the current owner id
		inline int8_t getOwner() {return owner;}

	public:
		/*! Returns the corresponding number or letter (upper-case) associated with the specific physical hardware used by this
		 * abstract hardware. ('0'-'9' / 'A'-'Z')
		 */
		virtual char getCharCode() = 0;
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_ABSTRACTHARDWAREBASE_ABSTRACTHARDWAREBASE_H_ */
