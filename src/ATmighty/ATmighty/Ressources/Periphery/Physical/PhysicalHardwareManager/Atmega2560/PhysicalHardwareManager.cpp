/*!
 * This file implements the namespace \see PhysicalHardwareManager
 */

#include "Atmega2560PhysicalHardwareManager.h"

#include <util/atomic.h>
#include "ATmighty/Ressources/Periphery/Physical/IoPorts.h"
#include "ATmighty/Ressources/Periphery/Physical/Timer.h"
#include "ATmighty/Ressources/Periphery/Physical/Usart.h"
#include "ATmighty/Utilities/Logs/MessageLog.h"
#include "MessageLogPhrases.h"


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

		//Log message
		#if ATMIGHTY_MESSAGELOG_ENABLE == true
		PGM_P hwString = GetHardwareStringRepresentation<Hw>();
		if (returnBuf)
			MessageLog<>::DefaultInstance().log<LogLevel::Info>(true, Phrase_Physical, hwString, Phrase_AllocSucess, Phrase_By, id);
		else
			MessageLog<>::DefaultInstance().log<LogLevel::Warning>(true, Phrase_Failed, Phrase_AllocFail, Phrase_Physical, hwString, Phrase_By, id);
		#endif

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
			//Log message
			#if ATMIGHTY_MESSAGELOG_ENABLE == true
			MessageLog<>::DefaultInstance().log<LogLevel::Info>(true, Phrase_Physical, GetHardwareStringRepresentation<Hw>(), Phrase_FreeSucess, Phrase_By, (*hardware)->getOwner());
			#endif

			(*hardware)->free();
			AllocatedItems--;
			(*hardware) = nullptr;
		}
		#if ATMIGHTY_MESSAGELOG_ENABLE == true
		else
		{
			MessageLog<>::DefaultInstance().log<LogLevel::Warning>(true, Phrase_Failed, Phrase_FreeFail, Phrase_Physical, GetHardwareStringRepresentation<Hw>(), Phrase_By, '?');
		}
		#endif

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
