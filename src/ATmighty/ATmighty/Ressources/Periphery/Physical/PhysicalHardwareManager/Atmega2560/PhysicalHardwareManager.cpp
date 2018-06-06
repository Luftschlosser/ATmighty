/*!
 * This file implements the namespace \see PhysicalHardwareManager
 */

#include "PhysicalHardwareManager.h"

#include <util/atomic.h>
#include "ATmighty/Ressources/Periphery/Physical/IoPorts.h"
#include "ATmighty/Ressources/Periphery/Physical/Timer.h"
#include "ATmighty/Ressources/Periphery/Physical/Usart.h"
#include "ATmighty/Utilities/Logs/MessageLog.h"
#include "ATmighty/Utilities/LUTs/MessageLogPhrases.h"
#include "ATmighty/Utilities/LUTs/HardwareOwnerID.h"


//main implementation of PhysicalHardwareManager
namespace PhysicalHardwareManager
{
	//the total number of hardware-items managed by the PhysicalHardwareManager (update the value when adding new items!)
	const uint8_t TotalItems = 3;

	//the total number of currently allocated hardware items
	uint8_t AllocatedItems = 0;


	//Logging-helper-Methods
	#if ATMIGHTY_MESSAGELOG_ENABLE == true

	void LogAllocSuccess(PGM_P hardware, int8_t id)
	{
		PGM_P idString = OwnerID::GetOwnerIdDescription(id);

		if (idString)
		{
			MessageLog<>::DefaultInstance().log<LogLevel::Info>(true,
					MessageLogPhrases::Phrase_PhysicalHw,
					hardware,
					MessageLogPhrases::Phrase_AllocSucess,
					MessageLogPhrases::Phrase_By,
					idString);
		}
		else
		{
			MessageLog<>::DefaultInstance().log<LogLevel::Info>(true,
					MessageLogPhrases::Phrase_PhysicalHw,
					hardware,
					MessageLogPhrases::Phrase_AllocSucess,
					MessageLogPhrases::Phrase_ById,
					id);
		}
	}

	void LogAllocFail(PGM_P hardware,int8_t id, int8_t owner)
	{
		PGM_P idString = OwnerID::GetOwnerIdDescription(id);
		PGM_P ownerString = OwnerID::GetOwnerIdDescription(owner);

		if (idString)
		{
			if (ownerString)
			{
				MessageLog<>::DefaultInstance().log<LogLevel::Warning>(true,
						MessageLogPhrases::Phrase_Failed,
						MessageLogPhrases::Phrase_AllocFail,
						MessageLogPhrases::Phrase_PhysicalHw,
						hardware,
						MessageLogPhrases::Phrase_By,
						idString,
						MessageLogPhrases::Reason_AllocFail_InUse,
						MessageLogPhrases::Phrase_By,
						ownerString);
			}
			else
			{
				MessageLog<>::DefaultInstance().log<LogLevel::Warning>(true,
						MessageLogPhrases::Phrase_Failed,
						MessageLogPhrases::Phrase_AllocFail,
						MessageLogPhrases::Phrase_PhysicalHw,
						hardware,
						MessageLogPhrases::Phrase_By,
						idString,
						MessageLogPhrases::Reason_AllocFail_InUse,
						MessageLogPhrases::Phrase_ById,
						owner);
			}
		}
		else
		{
			if (ownerString)
			{
				MessageLog<>::DefaultInstance().log<LogLevel::Warning>(true,
						MessageLogPhrases::Phrase_Failed,
						MessageLogPhrases::Phrase_AllocFail,
						MessageLogPhrases::Phrase_PhysicalHw,
						hardware,
						MessageLogPhrases::Phrase_ById,
						id,
						MessageLogPhrases::Reason_AllocFail_InUse,
						MessageLogPhrases::Phrase_By,
						ownerString);

			}
			else
			{
				MessageLog<>::DefaultInstance().log<LogLevel::Warning>(true,
						MessageLogPhrases::Phrase_Failed,
						MessageLogPhrases::Phrase_AllocFail,
						MessageLogPhrases::Phrase_PhysicalHw,
						hardware,
						MessageLogPhrases::Phrase_ById,
						id,
						MessageLogPhrases::Reason_AllocFail_InUse,
						MessageLogPhrases::Phrase_ById,
						owner);
			}
		}
	}

	void LogFreeSuccess(PGM_P hardware, int8_t id)
	{
		PGM_P idString = OwnerID::GetOwnerIdDescription(id);

		if (idString)
		{
			MessageLog<>::DefaultInstance().log<LogLevel::Info>(true,
					MessageLogPhrases::Phrase_PhysicalHw,
					hardware,
					MessageLogPhrases::Phrase_FreeSucess,
					MessageLogPhrases::Phrase_By,
					idString);
		}
		else
		{
			MessageLog<>::DefaultInstance().log<LogLevel::Info>(true,
					MessageLogPhrases::Phrase_PhysicalHw,
					hardware,
					MessageLogPhrases::Phrase_FreeSucess,
					MessageLogPhrases::Phrase_ById,
					id);
		}
	}

	void LogFreeFail(PGM_P hardware)
	{
		MessageLog<>::DefaultInstance().log<LogLevel::Warning>(true,
				MessageLogPhrases::Phrase_Failed,
				MessageLogPhrases::Phrase_FreeFail,
				MessageLogPhrases::Phrase_PhysicalHw,
				hardware,
				MessageLogPhrases::Phrase_By,
				'?',
				MessageLogPhrases::Reason_FreeFail);
	}

	#endif


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
		if (returnBuf)
		{
			//Success-Message
			LogAllocSuccess(Hw::GetHardwareStringRepresentation(), id);
		}
		else
		{
			//Fail-Message
			LogAllocFail(Hw::GetHardwareStringRepresentation(), id, instance.getOwner());
		}
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
			//Success-Message
			#if ATMIGHTY_MESSAGELOG_ENABLE == true
			LogFreeSuccess(Hw::GetHardwareStringRepresentation(), (*hardware)->getOwner());
			#endif

			(*hardware)->free();
			AllocatedItems--;
			(*hardware) = nullptr;
		}
		#if ATMIGHTY_MESSAGELOG_ENABLE == true
		else
		{
			//Fail-Message
			LogFreeFail(Hw::GetHardwareStringRepresentation());
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
	template PortB* Alloc<PortB>(int8_t id);
	template int8_t GetOwner<PortB>();
	template void Free<PortB>(PortB **hardware);
	template PortC* Alloc<PortC>(int8_t id);
	template int8_t GetOwner<PortC>();
	template void Free<PortC>(PortC **hardware);
	template PortD* Alloc<PortD>(int8_t id);
	template int8_t GetOwner<PortD>();
	template void Free<PortD>(PortD **hardware);
	template PortE* Alloc<PortE>(int8_t id);
	template int8_t GetOwner<PortE>();
	template void Free<PortE>(PortE **hardware);
	template PortF* Alloc<PortF>(int8_t id);
	template int8_t GetOwner<PortF>();
	template void Free<PortF>(PortF **hardware);
}
