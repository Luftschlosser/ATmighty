/*!
 * This headerfile is µC-dependent, it should only be included indirectly via "PhysicalHardwareManager.h".
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_PHYSICALHARDWAREMANAGER_ATMEGA2560_PHYSICALHARDWAREMANAGER_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_PHYSICALHARDWAREMANAGER_ATMEGA2560_PHYSICALHARDWAREMANAGER_H_


#include <stdint.h>


///This namespace contains some functions to manage the exclusive access to the physical hardware wrapper-class-instances.
namespace PhysicalHardwareManager
{
	///Returns the number of currently allocated hardware-items
	inline uint8_t GetAllocatedHardwareItems();

	///Returns the total number of available physical hardware-items
	inline uint8_t GetTotalHardwareItems();

	/*!
	 * Allocate a specified physical hardware-item.
	 * The desired hardware item is defined by the template argument, which must be one of the physical hardware wrapper-classes.
	 * \param id The OwnerId which requests this hardware allocation (must not be 0, use positive numbers, negatives are reserved for internal use by ATmighty)
	 * \returns a pointer to the allocated hardware-item, or nullptr if the allocation failed
	 */
	template<class Hw> Hw* Alloc (int8_t id);

	/*!
	 * Request the OwnerId of a specified physical hardware-item.
	 * The desired hardware item is defined by the template argument, which must be one of the physical hardware wrapper-classes.
	 * \returns The Id of the current owner of this hardware-item, or 0 if the hardware is free (currently unused)
	 */
	template<class Hw> int8_t GetOwner();

	/*!
	 * Frees an allocated physical hardware-item.
	 * The Type of the hardware-item to free must be defined by the template argument, which must be one of the physical hardware wrapper-classes.
	 * In this case the compiler is able to implicitly substitute the template parameter, so it is not obligatory to explicitly provide one.
	 * \param hardware A pointer to the pointer to the actual hardware-item. This is needed in order to set the callers reference to the hardware-item
	 * to nullptr after a sucessfull free. This procedure is needed in order to prevent the caller from freeing a hardware-item twice.
	 */
	template<class Hw> void Free(Hw **hardware);
}


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_PHYSICALHARDWAREMANAGER_ATMEGA2560_PHYSICALHARDWAREMANAGER_H_ */
