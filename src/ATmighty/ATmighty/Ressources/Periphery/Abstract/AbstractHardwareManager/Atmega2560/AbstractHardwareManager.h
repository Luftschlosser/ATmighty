/*!
 * This headerfile is µC-dependent, it should only be included indirectly via "AbstractHardwareManager.h".
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_ABSTRACTHARDWAREMANAGER_ATMEGA2560_ABSTRACTHARDWAREMANAGER_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_ABSTRACTHARDWAREMANAGER_ATMEGA2560_ABSTRACTHARDWAREMANAGER_H_


#include <stdint.h>

//forward-declare necessary classes
class AbstractIoPort;


///This namespace contains some functions to manage the exclusive access to the abstract hardware class-instances.
class AbstractHardwareManager
{
	private:
		int8_t owner;

	public:
		/*!
		 * Default Constructor.
		 * \param ownerId The ID of the user who requests this hardware allocation (must not be 0, use positive numbers, negatives are reserved for internal use by ATmighty)
		 */
		AbstractHardwareManager(int8_t ownerId);

		/*!
		 * Allocate a specific abstract hardware-item.
		 * The desired hardware item is defined by the template argument, which must be one of the abstract hardware classes.
		 * An hardware item allocated by this routine can be used faster than general abstract hardware instances, as the virtual function-calls don't need
		 * to be dereferenced via the v-table when the reference is stored as a pointer to the final class type.
		 * \returns a pointer to the allocated hardware-item, or nullptr if the allocation failed
		 */
		template<class Hw> Hw* allocItem();

		/*!
		 * Allocate any unused IoPort.
		 * Functioncalls will have an overhead of *x* cycles, as the virtual functions from the returned interface-type need to be dereferenced for each call.
		 * \returns a pointer to an abstract allocated IoPort-instance or nullptr if the allocation failed, which happens when all IoPorts are already in use.
		 */
		AbstractIoPort* allocIoPort();

		/*!
		 * Frees an allocated abstract hardware-item.
		 * The hardware item-type to free is defined by the template argument, which must be one of the abstract hardware classes.
		 * This is necessary because of the double-indirection, as this function otherwise would be able to substitute the derived base-instance with another
		 * derived instance, which would violate the Liskov Substition Principle.
		 * In this case the compiler is able to implicitly substitute the template parameter, so it is not obligatory to explicitly provide one.
		 * \param hardware A pointer to the pointer to the actual hardware-item. This is needed in order to set the callers reference to the hardware-item
		 * to nullptr after a sucessfull free. This procedure is needed in order to prevent the caller from freeing a hardware-item twice.
		 */
		template<class Hw> void freeItem(Hw **hardware);
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_ABSTRACTHARDWAREMANAGER_ATMEGA2560_ABSTRACTHARDWAREMANAGER_H_ */
