/*!
 * This file implements the namespace \see PhysicalHardwareManager
 */

#include "Atmega2560PhysicalHardwareManager.h"

#include "ATmighty/Ressources/Periphery/Physical/IoPorts.h"
#include "ATmighty/Ressources/Periphery/Physical/Timer.h"
#include "ATmighty/Ressources/Periphery/Physical/Usart.h"

#include <util/atomic.h>


namespace PhysicalHardwareManager
{
	//the total number of hardware-items managed by the PhysicalHardwareManager (update the value when adding new items!)
	const uint8_t TotalItems = 3;

	//the total number of currently allocated hardware items
	uint8_t AllocatedItems = 0;


	//Function-implementations:
	uint8_t GetTotalHardwareItems()
	{
		return TotalItems;
	}
	uint8_t GetAllocatedHardwareItems()
	{
		return AllocatedItems;
	}
	template<class Hw> Hw* Alloc (int8_t id)
	{
		Hw& instance = Hw::GetInstance();
		Hw* returnBuf;

		if (id == 0) {id = -1;} //validate ownerID

		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			if (instance.isFree())
			{
				instance.allocate(id);
				AllocatedItems++;
				returnBuf = &instance;
			}
			else
			{
				returnBuf = nullptr;
			}
		}

		//add log-message
		return returnBuf;
	}
	template<class Hw> int8_t GetOwner()
	{
		return Hw::GetInstance().getOwner();
	}
	template<class Hw> void Free(Hw **hardware)
	{
		if (hardware != nullptr && (*hardware) != nullptr)
		{
			(*hardware)->free();
			AllocatedItems--;
			(*hardware) = nullptr;
		}
		return;
	}


	//The explizitely instantiated versions of the template-functions (update the list when adding new types!)
	template Timer0* Alloc<Timer0>(int8_t id);
	template int8_t GetOwner<Timer0>();
	template void Free<Timer0>(Timer0 **hardware);

	template Usart0* Alloc<Usart0>(int8_t id);
	template int8_t GetOwner<Usart0>();
	template void Free<Usart0>(Usart0 **hardware);

	template PortA* Alloc<PortA>(int8_t id);
	template int8_t GetOwner<PortA>();
	template void Free<PortA>(PortA **hardware);
}
