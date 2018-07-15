/*
 * This headerfile is µC-dependent, it should only be included by ATmighty internals
 * This headerfile defines several string-literals and helper function which are needed to correctly log the activities ATmighty
 */

#ifndef ATMIGHTY_UTILITIES_LUTS_MESSAGELOGPHRASES_H_
#define ATMIGHTY_UTILITIES_LUTS_MESSAGELOGPHRASES_H_


#if ATMIGHTY_MESSAGELOG_ENABLE == true

#include <avr/pgmspace.h>


namespace MessageLogPhrases
{
	//define general hardware-allocation/-freeing phrases
	const char Phrase_PhysicalHw[] PROGMEM = "phys.HW ";
	const char Phrase_AbstractHw[] PROGMEM = "abst.HW ";
	const char Phrase_AllocSucess[] PROGMEM = " allocated";
	const char Phrase_AllocFail[] PROGMEM = "allocation of ";
	const char Phrase_FreeSucess[] PROGMEM = " freed";
	const char Phrase_FreeFail[] PROGMEM = "freeing of ";
	const char Phrase_Failed[] PROGMEM = "failed ";
	const char Phrase_By[] PROGMEM = " by ";
	const char Phrase_ById[] PROGMEM = " by ID ";

	//define specific reasons for hardware allocation/freeing problems
	const char Reason_FreeFail[] PROGMEM = ": Already free";
	const char Reason_AllocFail_InUse[] PROGMEM = ": In use";
	const char Reason_AllocFail_Dependency[] PROGMEM = ": Dependency not free";
	const char Reason_AllUsed[] PROGMEM = ": All in use";

	//Tool-Specific texts
	static const char Text_StopwatchCalibration[] PROGMEM = "Calibrating Stopwatch: Offset ";

	static const char Text_PeriodicTriggerValuesSet[] PROGMEM = "Periodic Trigger set Prescalar/CounterTop as: ";
	static const char Text_PeriodicTriggerInvalidTimer[] PROGMEM = "Periodic Trigger given Timer not suitable!";
	static const char Text_PeriodicTriggerApproximated[] PROGMEM = "Periodic Trigger interval-time was approximated: Offset ";
	static const char Text_PeriodicTriggerUnapproximatable[] PROGMEM = "Periodic Trigger couldn't approximate interval-time: Maximum ";

	static const char Text_UnitTimerSteps[] PROGMEM = " timersteps";
	static const char Text_UnitSeconds[] PROGMEM = " seconds";
	static const char Text_UnitMilliseconds[] PROGMEM = " milliseconds";
	static const char Text_UnitMicroseconds[] PROGMEM = " microseconds";
	static const char Text_UnitHertz[] PROGMEM = " hertz";
}

#endif


#endif /* ATMIGHTY_UTILITIES_LUTS_MESSAGELOGPHRASES_H_ */
