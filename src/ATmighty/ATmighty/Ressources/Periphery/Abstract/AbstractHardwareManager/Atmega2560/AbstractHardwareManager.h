/*!
 * This headerfile is µC-dependent, it should only be included indirectly via "AbstractHardwareManager.h".
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_ABSTRACTHARDWAREMANAGER_ATMEGA2560_ABSTRACTHARDWAREMANAGER_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_ABSTRACTHARDWAREMANAGER_ATMEGA2560_ABSTRACTHARDWAREMANAGER_H_


#include <stdint.h>

//forward-declare necessary classes
class IoPort;
class AbstractHardwareBase;


///This namespace contains some functions to manage the exclusive access to the abstract hardware class-instances.
class AbstractHardwareManager
{
	public:
		/*!
		 * Allocate a specific abstract hardware-item.
		 * The desired hardware item is defined by the template argument, which must be one of the abstract hardware classes
		 * An hardware item allocated by this routine can be used faster than general abstract hardware instances, as the virtual function-calls don't need
		 * to be dereferenced via the v-table when the reference is stored as a pointer to a final class type.
		 * \param id The OwnerId which requests this hardware allocation (must not be 0, use positive numbers, negatives are reserved for internal use by ATmighty)
		 * \returns a pointer to the allocated hardware-item, or nullptr if the allocation failed
		 */
		template<class Hw> Hw* Alloc (int8_t id);

		/*!
		 * Allocated any unused IoPort.
		 * Functioncalls will have an overhead of *x* cycles, as the virtual functions from the returned interface-type need to be dereferenced for each call.
		 * \param id The OwnerId which requests this hardware allocation (must not be 0, use positive numbers, negatives are reserved for internal use by ATmighty)
		 * \returns a pointer to an abstract allocated IoPort-instance or nullptr if the allocation failed, which happens when all IoPorts are already in use.
		 */
		IoPort* AllocIoPort(int8_t id);

		/*!
		 * Frees an allocated abstract hardware-item.
		 * \param hardware A pointer to the pointer to the actual hardware-item. This is needed in order to set the callers reference to the hardware-item
		 * to nullptr after a sucessfull free. This procedure is needed in order to prevent the caller from freeing a hardware-item twice.
		 */
		void Free(AbstractHardwareBase **hardware);
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_ABSTRACTHARDWAREMANAGER_ATMEGA2560_ABSTRACTHARDWAREMANAGER_H_ */
