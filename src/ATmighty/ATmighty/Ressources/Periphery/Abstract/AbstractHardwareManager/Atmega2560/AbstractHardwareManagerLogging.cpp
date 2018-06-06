/*!
 * This file implements the static logging-functions of class \see AbstractHardwareManager
 */


#include "AbstractHardwareManager.h"
#include "ATmighty/Utilities/Logs/MessageLog.h"
#include "ATmighty/Utilities/LUTs/MessageLogPhrases.h"
#include "ATmighty/Utilities/LUTs/HardwareOwnerID.h"


#if ATMIGHTY_MESSAGELOG_ENABLE == true

	void AbstractHardwareManager::logAllocSuccess(PGM_P hardware, char hwCode, int8_t id)
	{
		PGM_P idString = OwnerID::GetOwnerIdDescription(id);

		if (idString)
		{
			MessageLog<>::DefaultInstance().log<LogLevel::Info>(true,
					MessageLogPhrases::Phrase_AbstractHw,
					hardware,
					'[', hwCode, ']',
					MessageLogPhrases::Phrase_AllocSucess,
					MessageLogPhrases::Phrase_By,
					idString);
		}
		else
		{
			MessageLog<>::DefaultInstance().log<LogLevel::Info>(true,
					MessageLogPhrases::Phrase_AbstractHw,
					hardware,
					'[', hwCode, ']',
					MessageLogPhrases::Phrase_AllocSucess,
					MessageLogPhrases::Phrase_ById,
					id);
		}
	}

	void AbstractHardwareManager::logAllocSuccess(PGM_P hardware, char hwCode1, char hwCode2, int8_t id)
	{
		PGM_P idString = OwnerID::GetOwnerIdDescription(id);

		if (idString)
		{
			MessageLog<>::DefaultInstance().log<LogLevel::Info>(true,
					MessageLogPhrases::Phrase_AbstractHw,
					hardware,
					'[', hwCode1, hwCode2, ']',
					MessageLogPhrases::Phrase_AllocSucess,
					MessageLogPhrases::Phrase_By,
					idString);
		}
		else
		{
			MessageLog<>::DefaultInstance().log<LogLevel::Info>(true,
					MessageLogPhrases::Phrase_AbstractHw,
					hardware,
					'[', hwCode1, hwCode2, ']',
					MessageLogPhrases::Phrase_AllocSucess,
					MessageLogPhrases::Phrase_ById,
					id);
		}
	}

	void AbstractHardwareManager::logAllocFailDependency(PGM_P hardware, char hwCode, int8_t id)
	{
		PGM_P idString = OwnerID::GetOwnerIdDescription(id);

		if (idString)
		{
			MessageLog<>::DefaultInstance().log<LogLevel::Warning>(true,
					MessageLogPhrases::Phrase_Failed,
					MessageLogPhrases::Phrase_AllocFail,
					MessageLogPhrases::Phrase_AbstractHw,
					hardware,
					'[', hwCode, ']',
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
					hardware,
					'[', hwCode, ']',
					MessageLogPhrases::Phrase_ById,
					id,
					MessageLogPhrases::Reason_AllocFail_Dependency);

		}
	}

	void AbstractHardwareManager::logAllocFailDependency(PGM_P hardware, char hwCode1, char hwCode2, int8_t id)
	{
		PGM_P idString = OwnerID::GetOwnerIdDescription(id);

		if (idString)
		{
			MessageLog<>::DefaultInstance().log<LogLevel::Warning>(true,
					MessageLogPhrases::Phrase_Failed,
					MessageLogPhrases::Phrase_AllocFail,
					MessageLogPhrases::Phrase_AbstractHw,
					hardware,
					'[', hwCode1, hwCode2, ']',
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
					hardware,
					'[', hwCode1, hwCode2, ']',
					MessageLogPhrases::Phrase_ById,
					id,
					MessageLogPhrases::Reason_AllocFail_Dependency);

		}
	}

	void AbstractHardwareManager::logAllocFailUsed(PGM_P hardware, char hwCode, int8_t id, int8_t owner)
	{
		PGM_P idString = OwnerID::GetOwnerIdDescription(id);
		PGM_P userString = OwnerID::GetOwnerIdDescription(owner);

		if (idString)
		{
			if (userString)
			{
				MessageLog<>::DefaultInstance().log<LogLevel::Warning>(true,
						MessageLogPhrases::Phrase_Failed,
						MessageLogPhrases::Phrase_AllocFail,
						MessageLogPhrases::Phrase_AbstractHw,
						hardware,
						'[', hwCode, ']',
						MessageLogPhrases::Phrase_By,
						idString,
						MessageLogPhrases::Reason_AllocFail_InUse,
						MessageLogPhrases::Phrase_By,
						userString);
			}
			else
			{
				MessageLog<>::DefaultInstance().log<LogLevel::Warning>(true,
						MessageLogPhrases::Phrase_Failed,
						MessageLogPhrases::Phrase_AllocFail,
						MessageLogPhrases::Phrase_AbstractHw,
						hardware,
						'[', hwCode, ']',
						MessageLogPhrases::Phrase_By,
						idString,
						MessageLogPhrases::Reason_AllocFail_InUse,
						MessageLogPhrases::Phrase_ById,
						owner);
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
						hardware,
						'[', hwCode, ']',
						MessageLogPhrases::Phrase_ById,
						id,
						MessageLogPhrases::Reason_AllocFail_InUse,
						MessageLogPhrases::Phrase_By,
						userString);
			}
			else
			{
				MessageLog<>::DefaultInstance().log<LogLevel::Warning>(true,
						MessageLogPhrases::Phrase_Failed,
						MessageLogPhrases::Phrase_AllocFail,
						MessageLogPhrases::Phrase_AbstractHw,
						hardware,
						'[', hwCode, ']',
						MessageLogPhrases::Phrase_ById,
						id,
						MessageLogPhrases::Reason_AllocFail_InUse,
						MessageLogPhrases::Phrase_ById,
						owner);
			}
		}
	}

	void AbstractHardwareManager::logAllocFailUsed(PGM_P hardware, char hwCode1, char hwCode2, int8_t id, int8_t owner)
	{
		PGM_P idString = OwnerID::GetOwnerIdDescription(id);
		PGM_P userString = OwnerID::GetOwnerIdDescription(owner);

		if (idString)
		{
			if (userString)
			{
				MessageLog<>::DefaultInstance().log<LogLevel::Warning>(true,
						MessageLogPhrases::Phrase_Failed,
						MessageLogPhrases::Phrase_AllocFail,
						MessageLogPhrases::Phrase_AbstractHw,
						hardware,
						'[', hwCode1, hwCode2, ']',
						MessageLogPhrases::Phrase_By,
						idString,
						MessageLogPhrases::Reason_AllocFail_InUse,
						MessageLogPhrases::Phrase_By,
						userString);
			}
			else
			{
				MessageLog<>::DefaultInstance().log<LogLevel::Warning>(true,
						MessageLogPhrases::Phrase_Failed,
						MessageLogPhrases::Phrase_AllocFail,
						MessageLogPhrases::Phrase_AbstractHw,
						hardware,
						'[', hwCode1, hwCode2, ']',
						MessageLogPhrases::Phrase_By,
						idString,
						MessageLogPhrases::Reason_AllocFail_InUse,
						MessageLogPhrases::Phrase_ById,
						owner);
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
						hardware,
						'[', hwCode1, hwCode2, ']',
						MessageLogPhrases::Phrase_ById,
						id,
						MessageLogPhrases::Reason_AllocFail_InUse,
						MessageLogPhrases::Phrase_By,
						userString);
			}
			else
			{
				MessageLog<>::DefaultInstance().log<LogLevel::Warning>(true,
						MessageLogPhrases::Phrase_Failed,
						MessageLogPhrases::Phrase_AllocFail,
						MessageLogPhrases::Phrase_AbstractHw,
						hardware,
						'[', hwCode1, hwCode2, ']',
						MessageLogPhrases::Phrase_ById,
						id,
						MessageLogPhrases::Reason_AllocFail_InUse,
						MessageLogPhrases::Phrase_ById,
						owner);
			}
		}
	}

	void AbstractHardwareManager::logFreeSuccess(PGM_P hardware, char hwCode, int8_t id)
	{
		PGM_P idString = OwnerID::GetOwnerIdDescription(id);

		if (idString)
		{
			MessageLog<>::DefaultInstance().log<LogLevel::Info>(true,
					MessageLogPhrases::Phrase_AbstractHw,
					hardware,
					'[', hwCode,']',
					MessageLogPhrases::Phrase_FreeSucess,
					MessageLogPhrases::Phrase_By,
					idString);
		}
		else
		{
			MessageLog<>::DefaultInstance().log<LogLevel::Info>(true,
					MessageLogPhrases::Phrase_AbstractHw,
					hardware,
					'[', hwCode,']',
					MessageLogPhrases::Phrase_FreeSucess,
					MessageLogPhrases::Phrase_ById,
					id);
		}
	}

	void AbstractHardwareManager::logFreeSuccess(PGM_P hardware, char hwCode1, char hwCode2, int8_t id)
	{
		PGM_P idString = OwnerID::GetOwnerIdDescription(id);

		if (idString)
		{
			MessageLog<>::DefaultInstance().log<LogLevel::Info>(true,
					MessageLogPhrases::Phrase_AbstractHw,
					hardware,
					'[', hwCode1, hwCode2,']',
					MessageLogPhrases::Phrase_FreeSucess,
					MessageLogPhrases::Phrase_By,
					idString);
		}
		else
		{
			MessageLog<>::DefaultInstance().log<LogLevel::Info>(true,
					MessageLogPhrases::Phrase_AbstractHw,
					hardware,
					'[', hwCode1, hwCode2,']',
					MessageLogPhrases::Phrase_FreeSucess,
					MessageLogPhrases::Phrase_ById,
					id);
		}
	}

	void AbstractHardwareManager::logFreeFail(PGM_P hardware, int8_t id)
	{
		MessageLog<>::DefaultInstance().log<LogLevel::Warning>(true,
				MessageLogPhrases::Phrase_Failed,
				MessageLogPhrases::Phrase_FreeFail,
				MessageLogPhrases::Phrase_AbstractHw,
				hardware,
				'[', '?',']',
				MessageLogPhrases::Phrase_ById,
				id,
				MessageLogPhrases::Reason_FreeFail);
	}

#endif

