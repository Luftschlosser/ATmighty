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
	//define string-representations for hardware classes
	const char Hw_Undefined[] PROGMEM = "?";
	const char Hw_Timer0[] PROGMEM = "Timer0";
	const char Hw_Usart0[] PROGMEM = "Usart0";
	const char Hw_PortA[] PROGMEM = "PortA";
	const char Hw_PortB[] PROGMEM = "PortB";
	const char Hw_PortC[] PROGMEM = "PortC";
	const char Hw_PortD[] PROGMEM = "PortD";
	const char Hw_PortE[] PROGMEM = "PortE";
	const char Hw_PortF[] PROGMEM = "PortF";
	const char Hw_IoPort[] PROGMEM = "IoPort";
	const char Hw_IoPin[] PROGMEM = "IoPin";


	//define general hardware-allocation/-freeing phrases
	const char Phrase_PhysicalHw[] PROGMEM = "phys.HW ";
	const char Phrase_AbstractHw[] PROGMEM = "abst.HW ";
	const char Phrase_AllocSucess[] PROGMEM = " allocated";
	const char Phrase_AllocFail[] PROGMEM = "allocation of ";
	const char Phrase_FreeSucess[] PROGMEM = " freed";
	const char Phrase_FreeFail[] PROGMEM = "freeing of ";
	const char Phrase_Failed[] PROGMEM = "failed ";
	const char Phrase_By[] PROGMEM = " by ";
	const char Phrase_Id[] PROGMEM = "ID ";

	//define specific reasons for hardware allocation/freeing problems
	const char Reason_FreeFail[] PROGMEM = ": Already free";
	const char Reason_AllocFail_InUse[] PROGMEM = ": In use by ";
	const char Reason_AllocFail_Dependency[] PROGMEM = ": Dependency not free";
	const char Reason_AllUsed[] PROGMEM = ": All in use";
}

#endif


#endif /* ATMIGHTY_UTILITIES_LUTS_MESSAGELOGPHRASES_H_ */
