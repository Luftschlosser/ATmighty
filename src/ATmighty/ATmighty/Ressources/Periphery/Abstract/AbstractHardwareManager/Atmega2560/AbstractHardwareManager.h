/*!
 * This headerfile is µC-dependent, it should only be included indirectly via "AbstractHardwareManager.h".
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_ABSTRACTHARDWAREMANAGER_ATMEGA2560_ABSTRACTHARDWAREMANAGER_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_ABSTRACTHARDWAREMANAGER_ATMEGA2560_ABSTRACTHARDWAREMANAGER_H_


#include <stdint.h>
#include "ATmighty/Ressources/Periphery/Abstract/IoPorts.h"
#include "ATmighty/Ressources/Periphery/Abstract/IoPins.h"
#include "ATmighty/Ressources/Periphery/Abstract/Timer.h"
#include "Config/MessageLogConfig.h"


///This class contains some methods to manage the exclusive access to the abstract hardware class-instances.
class AbstractHardwareManager
{
	private:
		///The owner-id of this AbstractHardwareManager-instance. All Allocation done by this instance are executed using this owner-id.
		int8_t owner;

		/*!
		 * Allocate a specific abstract hardware-item.
		 * The desired hardware item is defined by the template argument, which must be one of the abstract hardware classes.
		 * An hardware item allocated by this routine can be used faster than general abstract hardware instances, as the virtual function-calls don't need
		 * to be dereferenced via the v-table when the reference is stored as a pointer to the final class type.
		 * If the specific abstract hardware to allocate is known at compile-time, this function should be preferred over the general parameterized allocator-functions.
		 * \returns a pointer to the allocated hardware-item, or nullptr if the allocation failed
		 */
		template<class Hw> Hw* allocItem();

		//general freeing-method (to save memory for all the specific abstract hardware-types to free
		template<class ToFree> static void Free(ToFree** hardware, int8_t owner);
		//Logging-helper-Methods
		#if ATMIGHTY_MESSAGELOG_ENABLE == true
		static void logAllocSuccess(PGM_P hardware, char hwCode, int8_t id);
		static void logAllocSuccess(PGM_P hardware, char hwCode1, char hwCode2, int8_t id);
		static void logAllocFailDependency(PGM_P hardware, char hwCode, int8_t id);
		static void logAllocFailDependency(PGM_P hardware, char hwCode1, char hwCode2, int8_t id);
		static void logAllocFailUsed(PGM_P hardware, char hwCode, int8_t id, int8_t owner);
		static void logAllocFailUsed(PGM_P hardware, char hwCode1, char hwCode2, int8_t id, int8_t owner);
		static void logFreeSuccess(PGM_P hardware, char hwCode, int8_t id);
		static void logFreeSuccess(PGM_P hardware, char hwCode1, char hwCode2, int8_t id);
		static void logFreeFail(PGM_P hardware, int8_t id);
		#endif

	public:
		/*!
		 * Default Constructor.
		 * \param ownerId The ID of the user who requests this hardware allocation (must not be 0, use positive numbers, negatives are reserved for internal use by ATmighty)
		 */
		AbstractHardwareManager(int8_t ownerId);

		/*!
		 * Allocates an IoPort.
		 * If the specific abstract IoPort to allocate is known at compile-time, this function should be preferred over the general parameterized allocator-function.
		 * The letter ('A'-'L') of the Port to allocate are defined via the template parameter PortChar
		 * \returns a pointer to an abstract allocated IoPort-instance or nullptr if the allocation failed, which happens when the specified IoPort is already in use.
		 */
		template<char PortChar> inline SpecificIoPort<PortChar>* allocIoPort() {return allocItem<SpecificIoPort<PortChar>>();}

		/*!
		 * Allocates an IoPort.
		 * This general IoPort-Allocator should only be used when the specific IoPort to allocate is not known at compile-time!
		 * \param portChar the upper-case character which specifies the IoPort to allocate ('A'-'L')
		 * \returns a pointer to an abstract allocated IoPort-instance or nullptr if the allocation failed, which happens when the specified IoPort is already in use.
		 */
		GeneralIoPort* allocIoPort(char portChar);

		/*!
		 * Allocates an IoPin.
		 * If the specific abstract IoPin to allocate is known at compile-time, this function should be preferred over the general parameterized allocator-function.
		 * The letter ('A'-'L') and the pin-number (0-7) of the Port/Pin are defined via the template parameters PortChar and PinNumber
		 * \returns a pointer to an abstract allocated IoPin-instance or nullptr if the allocation failed, which happens when the specified IoPin is already in use.
		 */
		template<char PortChar, uint8_t PinNumber> SpecificIoPin<PortChar, PinNumber>* allocIoPin();

		/*!
		 * Allocates an IoPin.
		 * This general IoPin-Allocator should only be used when the specific IoPin to allocate is not known at compile-time!
		 * \param portChar the upper-case character which specifies the IoPort the IoPin to allocate is part of ('A'-'L')
		 * \param pinNumber the number of the IoPin to allocate within the IoPort (0-7)
		 * \returns a pointer to an abstract allocated IoPin-instance or nullptr if the allocation failed, which happens when the specified IoPin is already in use.
		 */
		GeneralIoPin* allocIoPin(char portChar, uint8_t pinNumber);

		/*!
		 * Allocates an 8bit Timer.
		 * The template parameter can be either used to specify the type of 8bit-Timer or an specific one to allocate. Possible values are:
		 * "AbstractTimer8bit"/Default:	When no explicit template parameter is provided, this method will return any free 8bit-Timer.
		 * "AbstractTimer8bitAsync":	This method will return any free 8bit-Timer which supports asynchronous operation.
		 * "AbstractTimer0":			This method will return an abstraction of Timer0 if currently available.
		 * "AbstractTimer2":			This method will return an abstraction of Timer2 if currently available.
		 * \returns a pointer to the allocated timer-instance on success, nullptr when the specified timer(s) could not be allocated.
		 */
		template<class TimerHw = AbstractTimer8bit> TimerHw* allocTimer8bit();

		/*!
		 * Allocates an 16bit Timer.
		 * The template parameter can be either used to specify the type of 16bit-Timer or an specific one to allocate. Possible values are:
		 * "AbstractTimer16bit"/Default:	When no explicit template parameter is provided, this method will return any free 16bit-Timer.
		 * "AbstractTimer1":				This method will return an abstraction of Timer1 if currently available.
		 * \returns a pointer to the allocated timer-instance on success, nullptr when the specified timer(s) could not be allocated.
		 */
		template<class TimerHw = AbstractTimer16bit> TimerHw* allocTimer16bit();

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
