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

	static const char Text_PeriodicTriggerValuesSet[] PROGMEM = "PeriodicTrigger set Prescalar/CounterTop as: ";
	static const char Text_PeriodicTriggerInvalidTimer[] PROGMEM = "PeriodicTrigger given Timer not suitable!";
	static const char Text_PeriodicTriggerApproximated[] PROGMEM = "PeriodicTrigger interval-time was approximated: Offset ";
	static const char Text_PeriodicTriggerUnapproximatable[] PROGMEM = "PeriodicTrigger couldn't approximate interval-time: Maximum ";

	static const char Text_VirtualTimerPoolSetup1[] PROGMEM = "VirtualTimerPool [Size ";
	static const char Text_VirtualTimerPoolSetup2[] PROGMEM = "] set up with counting-frequency: ";
	static const char Text_VirtualTimerPoolSetupFail[] PROGMEM = "VirtualTimerPool setup failed: Out of memory!";
	static const char Text_VirtualTimerPoolVirtualized8bit[] PROGMEM = "VirtualTimerPool virtualized new 8bit-Timer with ";
	static const char Text_VirtualTimerPoolVirtualizationFail[] PROGMEM = "VirtualTimerPool cannot virtualize new 8bit-Timer: Pool full!";
	static const char Text_VirtualTimerPoolFreed8bit[] PROGMEM = "VirtualTimerPool removed virtual 8bit-Timer.";
	static const char Text_VirtualTimerPoolFreeFail[] PROGMEM = "VirtualTimerPool cannot remove virtual 8bit-Timer: Wrong reference!";

	static const char Text_VirtualTimerSetupFail[] PROGMEM = "VirtualTimer setup failed: Out of memory!";
	static const char Text_VirtualTimerChannelMax[] PROGMEM = "VirtualTimer maximally supports 4 channels!";
	static const char Text_VirtualTimerStarted1[] PROGMEM = "VirtualTimer started with prescalar: ";
	static const char Text_VirtualTimerStarted2[] PROGMEM = ", Mode: ";
	static const char Text_VirtualTimerStarted3[] PROGMEM = ", Base-frequency: ";

	static const char Text_UnitTimerSteps[] PROGMEM = " timersteps";
	static const char Text_UnitSeconds[] PROGMEM = " s";
	static const char Text_UnitMilliseconds[] PROGMEM = " ms";
	static const char Text_UnitMicroseconds[] PROGMEM = " micros";
	static const char Text_UnitHertz[] PROGMEM = " hz";
	static const char Text_UnitChannels[] PROGMEM = " channels.";
}

#endif


#endif /* ATMIGHTY_UTILITIES_LUTS_MESSAGELOGPHRASES_H_ */
