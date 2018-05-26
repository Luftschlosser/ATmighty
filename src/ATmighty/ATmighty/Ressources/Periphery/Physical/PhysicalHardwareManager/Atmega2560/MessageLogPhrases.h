/*
 * This headerfile is µC-dependent, it should not be included by anyone else than Atmega2560/PhysicalHardwareManager.cpp
 * This headerfile defines several string-literals and helper function which are needed to correctly log the activities of the
 * Atmega2560's PhysicalHardwareManager.
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_PHYSICALHARDWAREMANAGER_MESSAGELOGPHRASES_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_PHYSICALHARDWAREMANAGER_MESSAGELOGPHRASES_H_


#if ATMIGHTY_MESSAGELOG_ENABLE == true

#include <avr/pgmspace.h>


namespace PhysicalHardwareManager
{
	//define string-representations for physical hardware classes
	const char Hw_Undefined[] PROGMEM = "?";
	const char Hw_Timer0[] PROGMEM = "Timer0";
	const char Hw_Usart0[] PROGMEM = "Usart0";
	const char Hw_PortA[] PROGMEM = "PortA";
	const char Hw_PortB[] PROGMEM = "PortB";
	const char Hw_PortC[] PROGMEM = "PortC";
	const char Hw_PortD[] PROGMEM = "PortD";
	const char Hw_PortE[] PROGMEM = "PortE";
	const char Hw_PortF[] PROGMEM = "PortF";

	//define general phrases
	const char Phrase_Physical[] PROGMEM = "phys.HW ";
	const char Phrase_AllocSucess[] PROGMEM = " allocated";
	const char Phrase_AllocFail[] PROGMEM = "allocation of ";
	const char Phrase_FreeSucess[] PROGMEM = " freed";
	const char Phrase_FreeFail[] PROGMEM = "freeing of ";
	const char Phrase_Failed[] PROGMEM = "failed ";
	const char Phrase_By[] PROGMEM = " by ";
	const char Phrase_Id[] PROGMEM = "ID ";

	//helper-functions
	template<class T> inline PGM_P GetHardwareStringRepresentation() {return Hw_Undefined;}
	template<> inline PGM_P GetHardwareStringRepresentation<Timer0>() {return Hw_Timer0;}
	template<> inline PGM_P GetHardwareStringRepresentation<Usart0>() {return Hw_Usart0;}
	template<> inline PGM_P GetHardwareStringRepresentation<PortA>() {return Hw_PortA;}
	template<> inline PGM_P GetHardwareStringRepresentation<PortB>() {return Hw_PortB;}
	template<> inline PGM_P GetHardwareStringRepresentation<PortC>() {return Hw_PortC;}
	template<> inline PGM_P GetHardwareStringRepresentation<PortD>() {return Hw_PortD;}
	template<> inline PGM_P GetHardwareStringRepresentation<PortE>() {return Hw_PortE;}
	template<> inline PGM_P GetHardwareStringRepresentation<PortF>() {return Hw_PortF;}
}

#endif


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_PHYSICALHARDWAREMANAGER_MESSAGELOGPHRASES_H_ */
