/*
 * This file implements the function defined in HardwareOwnerID.h
 */

#include "HardwareOwnerID.h"


PGM_P GetOwnerIdDescription(int8_t id)
{
	static const uint8_t numberOfDescriptions = 5;

	//string definitions for logging owners (defined static in function, so that it is only included into final assembly when actually used)
	static const char Owner_Unused[] PROGMEM = "Nobody";
	static const char Owner_Default[] PROGMEM = "Default";
	static const char Owner_DirectAbstraction[] PROGMEM = "Abstraction";
	static const char Owner_AbstractionDependency[] PROGMEM = "abstr.Dependency";
	static const char Owner_MessageLogWriter[] PROGMEM = "MessageLogWriter";
	static PGM_P const Owner_Phrases[numberOfDescriptions] PROGMEM =
	{
			[0] = Owner_Unused,
			[1] = Owner_Default,
			[2] = Owner_DirectAbstraction,
			[3] = Owner_AbstractionDependency,
			[4] = Owner_MessageLogWriter
	};

	//actual function
	if (id <= 0) //negative
	{
		id = -id;
		if (id < numberOfDescriptions) //in defined range
		{
			return (PGM_P)pgm_read_word(&(Owner_Phrases[id]));
		}
	}
	return nullptr;
}
