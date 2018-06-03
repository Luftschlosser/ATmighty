/*!
 * This file implements the class \see AbstractHardwareManager
 */

#include "AbstractHardwareManager.h"

#include <util/atomic.h>
#include "ATmighty/Ressources/Periphery/Abstract/IoPorts.h"
#include "ATmighty/Ressources/Periphery/Abstract/IoPins.h"
#include "ATmighty/Utilities/Logs/MessageLog.h"
#include "ATmighty/Utilities/LUTs/MessageLogPhrases.h"
#include "ATmighty/Utilities/LUTs/HardwareOwnerID.h"


//Additional Helper-functions and explicitly instantiated template functions for Message-Logging
#if ATMIGHTY_MESSAGELOG_ENABLE == true
namespace MessageLogPhrases
{
	template<class T> inline PGM_P GetHardwareStringRepresentation() {return Hw_Undefined;}

	template<> inline PGM_P GetHardwareStringRepresentation<AbstractIoPort>() {return Hw_IoPort;}
	template<> inline PGM_P GetHardwareStringRepresentation<AbstractPortA>() {return Hw_IoPort;}

	template<> inline PGM_P GetHardwareStringRepresentation<AbstractIoPin>() {return Hw_IoPin;}
	template<> inline PGM_P GetHardwareStringRepresentation<AbstractPinA0>() {return Hw_IoPin;}
}
#endif



//main implementation of AbstractHardwareManager

AbstractHardwareManager::AbstractHardwareManager(int8_t ownerId) : owner(ownerId)
{
	if (owner == 0) //validate ownerID
	{
		owner = -1;
	}
}


template<class Hw> Hw* AbstractHardwareManager::allocItem ()
{
	Hw& instance = Hw::GetInstance();
	int8_t returnCode;

	//MessageLog Variables
	#if ATMIGHTY_MESSAGELOG_ENABLE == true
	PGM_P hwString = MessageLogPhrases::GetHardwareStringRepresentation<Hw>();
	PGM_P idString = OwnerID::GetOwnerIdDescription(owner);
	#endif

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		returnCode = instance.init(owner);
	}

	if (returnCode > 0) //abstract hardware is already in use
	{
		//Fail-Message
		#if ATMIGHTY_MESSAGELOG_ENABLE == true
		int8_t userId = instance.getOwner();
		PGM_P userString = OwnerID::GetOwnerIdDescription(userId);

		if (idString)
		{
			if (userString)
			{
				MessageLog<>::DefaultInstance().log<LogLevel::Warning>(true,
						MessageLogPhrases::Phrase_Failed,
						MessageLogPhrases::Phrase_AllocFail,
						MessageLogPhrases::Phrase_AbstractHw,
						hwString,
						'[', instance.getCharCode(),']',
						MessageLogPhrases::Phrase_By,
						idString,
						MessageLogPhrases::Reason_AllocFail_InUse,
						userString);
			}
			else
			{
				MessageLog<>::DefaultInstance().log<LogLevel::Warning>(true,
						MessageLogPhrases::Phrase_Failed,
						MessageLogPhrases::Phrase_AllocFail,
						MessageLogPhrases::Phrase_AbstractHw,
						hwString,
						'[', instance.getCharCode(),']',
						MessageLogPhrases::Phrase_By,
						idString,
						MessageLogPhrases::Reason_AllocFail_InUse,
						MessageLogPhrases::Phrase_Id,
						userId);
			}
		}
		else
		{
			if (userString)
			{
				MessageLog<>::DefaultInstance().log<LogLevel::Warning>(true,
						MessageLogPhrases::Phrase_Failed,
						MessageLogPhrases::Phrase_AllocFail,
						MessageLogPhrases::Phrase_AbstractHw,
						hwString,
						'[', instance.getCharCode(),']',
						MessageLogPhrases::Phrase_By,
						MessageLogPhrases::Phrase_Id,
						owner,
						MessageLogPhrases::Reason_AllocFail_InUse,
						userString);
			}
			else
			{
				MessageLog<>::DefaultInstance().log<LogLevel::Warning>(true,
						MessageLogPhrases::Phrase_Failed,
						MessageLogPhrases::Phrase_AllocFail,
						MessageLogPhrases::Phrase_AbstractHw,
						hwString,
						'[', instance.getCharCode(),']',
						MessageLogPhrases::Phrase_By,
						MessageLogPhrases::Phrase_Id,
						owner,
						MessageLogPhrases::Reason_AllocFail_InUse,
						MessageLogPhrases::Phrase_Id,
						userId);
			}
		}
		#endif

		return nullptr;
	}
	else if (returnCode <0) //physical hardware dependency could not be resolved
	{
		//Fail-Message
		#if ATMIGHTY_MESSAGELOG_ENABLE == true
		if (idString)
		{
			MessageLog<>::DefaultInstance().log<LogLevel::Warning>(true,
					MessageLogPhrases::Phrase_Failed,
					MessageLogPhrases::Phrase_AllocFail,
					MessageLogPhrases::Phrase_AbstractHw,
					hwString,
					'[', instance.getCharCode(),']',
					MessageLogPhrases::Phrase_By,
					idString,
					MessageLogPhrases::Reason_AllocFail_Dependency);
		}
		else
		{
			MessageLog<>::DefaultInstance().log<LogLevel::Warning>(true,
					MessageLogPhrases::Phrase_Failed,
					MessageLogPhrases::Phrase_AllocFail,
					MessageLogPhrases::Phrase_AbstractHw,
					hwString,
					'[', instance.getCharCode(),']',
					MessageLogPhrases::Phrase_By,
					MessageLogPhrases::Phrase_Id,
					owner,
					MessageLogPhrases::Reason_AllocFail_Dependency);

		}
		#endif

		return nullptr;
	}
	else
	{
		//Success-Message
		#if ATMIGHTY_MESSAGELOG_ENABLE == true
		if (idString)
		{
			MessageLog<>::DefaultInstance().log<LogLevel::Info>(true,
					MessageLogPhrases::Phrase_AbstractHw,
					hwString,
					'[', instance.getCharCode(),']',
					MessageLogPhrases::Phrase_AllocSucess,
					MessageLogPhrases::Phrase_By,
					idString);
		}
		else
		{
			MessageLog<>::DefaultInstance().log<LogLevel::Info>(true,
					MessageLogPhrases::Phrase_AbstractHw,
					hwString,
					'[', instance.getCharCode(),']',
					MessageLogPhrases::Phrase_AllocSucess,
					MessageLogPhrases::Phrase_By,
					MessageLogPhrases::Phrase_Id,
					owner);
		}
		#endif

		return &instance;
	}
}


AbstractIoPort* AbstractHardwareManager::allocIoPort()
{
	//Try PortA
	if (AbstractPortA::GetInstance().isFree())
	{
		AbstractPortA* ret = allocItem<AbstractPortA>();
		if (ret)
		{
			return ret;
		}
	}

	//Case when no IoPort could be allocated successfully
	#if ATMIGHTY_MESSAGELOG_ENABLE == true
	//log "All Ports in use"
	PGM_P idString = OwnerID::GetOwnerIdDescription(owner);

	if (idString)
	{
		MessageLog<>::DefaultInstance().log<LogLevel::Info>(true,
				MessageLogPhrases::Phrase_Failed,
				MessageLogPhrases::Phrase_AllocFail,
				MessageLogPhrases::Phrase_AbstractHw,
				MessageLogPhrases::GetHardwareStringRepresentation<AbstractPortA>(),
				MessageLogPhrases::Phrase_By,
				idString,
				MessageLogPhrases::Reason_AllUsed);
	}
	else
	{
		MessageLog<>::DefaultInstance().log<LogLevel::Info>(true,
				MessageLogPhrases::Phrase_Failed,
				MessageLogPhrases::Phrase_AllocFail,
				MessageLogPhrases::Phrase_AbstractHw,
				MessageLogPhrases::GetHardwareStringRepresentation<AbstractPortA>(),
				MessageLogPhrases::Phrase_By,
				MessageLogPhrases::Phrase_Id,
				owner,
				MessageLogPhrases::Reason_AllUsed);
	}
	#endif

	return nullptr;
}


template<class Hw> void AbstractHardwareManager::freeItem(Hw **hardware)
{
	//MessageLog Variables
	#if ATMIGHTY_MESSAGELOG_ENABLE == true
	PGM_P hwString = MessageLogPhrases::GetHardwareStringRepresentation<Hw>();
	#endif

	if (hardware != nullptr && (*hardware) != nullptr)
	{
		(*hardware)->exit();

		//Success-Message
		#if ATMIGHTY_MESSAGELOG_ENABLE == true
		PGM_P idString = OwnerID::GetOwnerIdDescription(owner);

		if (idString)
		{
			MessageLog<>::DefaultInstance().log<LogLevel::Info>(true,
					MessageLogPhrases::Phrase_AbstractHw,
					hwString,
					'[', (*hardware)->getCharCode(),']',
					MessageLogPhrases::Phrase_FreeSucess,
					MessageLogPhrases::Phrase_By,
					idString);
		}
		else
		{
			MessageLog<>::DefaultInstance().log<LogLevel::Info>(true,
					MessageLogPhrases::Phrase_AbstractHw,
					hwString,
					'[', (*hardware)->getCharCode(),']',
					MessageLogPhrases::Phrase_FreeSucess,
					MessageLogPhrases::Phrase_By,
					MessageLogPhrases::Phrase_Id,
					owner);
		}
		#endif

		(*hardware)=nullptr;
	}
	#if ATMIGHTY_MESSAGELOG_ENABLE == true
	else
	{
		//Fail-Message
		MessageLog<>::DefaultInstance().log<LogLevel::Warning>(true,
				MessageLogPhrases::Phrase_Failed,
				MessageLogPhrases::Phrase_FreeFail,
				MessageLogPhrases::Phrase_AbstractHw,
				hwString,
				'[', '?',']',
				MessageLogPhrases::Phrase_By,
				MessageLogPhrases::Phrase_Id,
				owner,
				MessageLogPhrases::Reason_FreeFail);
	}
	#endif
}


//The explizitely instantiated versions of the template-functions (update the list when adding new types!)
template AbstractPortA* AbstractHardwareManager::allocItem<AbstractPortA>();
template void AbstractHardwareManager::freeItem<AbstractIoPort>(AbstractIoPort **hardware);
template void AbstractHardwareManager::freeItem<AbstractPortA>(AbstractPortA **hardware);

template AbstractPinA0* AbstractHardwareManager::allocItem<AbstractPinA0>();
template void AbstractHardwareManager::freeItem<AbstractIoPin>(AbstractIoPin **hardware);
template void AbstractHardwareManager::freeItem<AbstractPinA0>(AbstractPinA0 **hardware);
