/*!
 * This headerfile is µC-dependent, it should only be included indirectly via "PhysicalHardwareManager.h".
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_PHYSICALHARDWAREMANAGER_ATMEGA2560PHYSICALHARDWAREMANAGER_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_PHYSICALHARDWAREMANAGER_ATMEGA2560PHYSICALHARDWAREMANAGER_H_


#include <avr/pgmspace.h>
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareBase/PhysicalHardwareBase.h"
#include "ATmighty/Ressources/Periphery/Physical/IoPorts.h"
#include "ATmighty/Ressources/Periphery/Physical/Timer.h"
#include "ATmighty/Ressources/Periphery/Physical/Usart.h"

/*!
 * This class manages the exclusive direct access to the physical periphery of the atmega2560 µC.
 */
class PhysicalHardwareManager
{
	private:
		//Counters to track the physical hardware usage:
		static const uint8_t totalHardwareItems = 1;		//The total number of physical hardware items.
		static volatile uint8_t allocatedHardwareItems;		//The total number of used (allocated) physical hardware items.
		int8_t instanceUsageRelation;						//Holds [#Allocations - #Frees] done by this PhysicalHardwareManager-instance.
															//(not really neccessary, but object has 1Byte size anyway)

		//The instances of the physical hardware items
		static PortA portA;
		static Timer0 timer0;
		static Usart0 usart0;


	public:
		///Default Constructor. Initializes a new PhysicalHardwareManager.
		inline PhysicalHardwareManager() : instanceUsageRelation(0) {}

		///Default Destructor
		inline ~PhysicalHardwareManager() {if(instanceUsageRelation != 0){/*log-message (warning)*/ asm("nop");}}


		///returns the total number of physical hardware item managed by this PhysicalHardwareManager
		inline static uint8_t getTotalHardwareItems() {return totalHardwareItems;}

		///returns the total number of free (currently unused) physical hardware items
		inline static uint8_t getTotalFreeHardwareItems() {return (totalHardwareItems - allocatedHardwareItems);}

		///returns the total number of used (currently allocated) physical hardware items
		inline static uint8_t getTotalAllocatedHardwareItems() {return allocatedHardwareItems;}

		///return the number of allocations minus the number of frees done with this PhysicalHardwareManager-instance
		inline int8_t getUsageRelation() {return instanceUsageRelation;}


		/*
		 * Gets the Owner-ID of the physical hardware item defined by the template class.
		 * \returns the Owner-ID of the specified item, which is 0 if the item is currently unused.
		 */
		template<class Hw> inline int8_t getOwnerId();

		/*
		 * Allocates a physical hardware-item defined by the template class.
		 * \param ownerID The ID of the user who requests the allocation. Negative numbers are reserved to be used by ATmighty only.
		 * Positive numbers are to be used by other users. This value must not be 0. If ownerID==0 the value will be changed to -1.
		 * \returns A pointer to the requested physical hardware-item or nullptr if the item is currently in use by another owner.
		 */
		template<class Hw> Hw* alloc(int8_t ownerID);

		/*
		 * Frees an allocated physical hardware item.
		 * \param hardware A pointer to the pointer to the actual physical hardware item. This is necessary, as the function will set
		 * the caller's reference to the item to nullptr after freeing the associated hardware item. This procedure will prevent the caller
		 * from freeing the hardware twice.
		 */
		void free(PhysicalHardwareBase **hardware);

		//Overloaded derivates of free() for PhysicalHardwareBase-childs.
		inline void free(PortA **portA) {/*add log-message*/free((PhysicalHardwareBase**)portA);}
		inline void free(Timer0 **timer0) {/*add log-message*/free((PhysicalHardwareBase**)timer0);}
		inline void free(Usart0 **usart0) {/*add log-message*/free((PhysicalHardwareBase**)usart0);}
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_PHYSICALHARDWAREMANAGER_ATMEGA2560PHYSICALHARDWAREMANAGER_H_ */
