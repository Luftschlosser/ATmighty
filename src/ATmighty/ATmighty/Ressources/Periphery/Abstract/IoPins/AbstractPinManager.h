/*!
 * This headerfile defines the class \see AbstractPinManager which holds some helper function to keep track of used/free pins.
 * This headerfile should only be included by AbstractPin-Classes!
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPINS_ABSTRACTPINMANAGER_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPINS_ABSTRACTPINMANAGER_H_


#include <stdint.h>


/*!
 * This class holds some static helper-functions which keep track of used pins on each IoPort
 * and manage the allocation/freeing of the respective physical Hardware.
 * This class must only be used by the init()/exit()-functions of the AbstractIoPins! NO MESSING AROUND!
 * The template-Parameter can be any of the physical IoPort-classes,
 * it is used to instantiate a unique set of the static-members for each IoPort.
 * This class cannot be instantiated, only its static methods should be accessed!
 */
template <class PhysicalPort> class AbstractPinManager
{
	private:
		/// A static pointer to the physical Port from which the pins will get used
		static PhysicalPort* PhysicalReference;

		/// An 8-bit mask which defines the usage-state for each of the port's IoPins
		static uint8_t UsageMask; //0=free, 1=used

		/// Private default-constructor to prevent instantiation!
		AbstractPinManager() {}

	public:

		/*!
		 * returns true if the specified pin is currently free (does not imply that the physical Port is free too)
		 * \param pin 0-7
		 */
		static inline bool IsPinFree(uint8_t pin)
		{
			return !(UsageMask & (1<<pin));
		}

		/*!
		 * Tries to allocate a specified pin.
		 * \param pin the pin to allocate 0-7
		 * \returns 0 on sucess, >0 is this pin is already in use, <0 if the physical port is already in use
		 */
		static int8_t AllocPin(uint8_t pin);

		/*!
		 * Frees an allocated pin. If the entire physical IoPort is unused after this operation, the allocated physical IoPort will be freed too.
		 * \param pin the pin to free, 0-7
		 */
		static void FreePin(uint8_t pin);
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPINS_ABSTRACTPINMANAGER_H_ */
