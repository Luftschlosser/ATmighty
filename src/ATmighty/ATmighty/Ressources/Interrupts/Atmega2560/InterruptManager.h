/*!
 * This headerfile includes a namespace with ISR-routines and dynamic Listener-instances for each interrupt source of the atmega2560.
 * This headerfile is µC-dependent, it should only be included via the general InterruptManager.h-file.
 */

#ifndef ATMIGHTY_RESSOURCES_INTERRUPTS_ATMEGA2560_INTERRUPTMANAGER_H_
#define ATMIGHTY_RESSOURCES_INTERRUPTS_ATMEGA2560_INTERRUPTMANAGER_H_


#include "Config/InterruptConfig.h"


//forward declaration of "Listener"
class Listener;

/*!
 * This namespace includes functions to set Listeners for each managed Interrupt-source.
 * Which Interrupts get managed by this InterruptManager can be configured in "InterruptConfig.h"
 */
namespace InterruptManager
{
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV0
	///Sets a Listener which gets triggered on a Timer0-Overflow-Interrupt
	void setTOV0(Listener* listener);
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF0A
	///Sets a Listener which gets triggered on a Timer0-OutputCompareMatchA-Interrupt
	void setOCF0A(Listener* listener);
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF0B
	///Sets a Listener which gets triggered on a Timer0-OutputCompareMatchB-Interrupt
	void setOCF0B(Listener* listener);
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV1
	///Sets a Listener which gets triggered on a Timer1-Overflow-Interrupt
	void setTOV1(Listener* listener);
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF1A
	///Sets a Listener which gets triggered on a Timer1-OutputCompareMatchA-Interrupt
	void setOCF1A(Listener* listener);
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF1B
	///Sets a Listener which gets triggered on a Timer1-OutputCompareMatchB-Interrupt
	void setOCF1B(Listener* listener);
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF1C
	///Sets a Listener which gets triggered on a Timer1-OutputCompareMatchB-Interrupt
	void setOCF1C(Listener* listener);
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF1
	///Sets a Listener which gets triggered on a Timer1-OutputCompareMatchB-Interrupt
	void setICF1(Listener* listener);
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV2
	///Sets a Listener which gets triggered on a Timer2-Overflow-Interrupt
	void setTOV2(Listener* listener);
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF2A
	///Sets a Listener which gets triggered on a Timer2-OutputCompareMatchA-Interrupt
	void setOCF2A(Listener* listener);
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF2B
	///Sets a Listener which gets triggered on a Timer2-OutputCompareMatchB-Interrupt
	void setOCF2B(Listener* listener);
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV3
	///Sets a Listener which gets triggered on a Timer3-Overflow-Interrupt
	void setTOV3(Listener* listener);
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF3A
	///Sets a Listener which gets triggered on a Timer3-OutputCompareMatchA-Interrupt
	void setOCF3A(Listener* listener);
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF3B
	///Sets a Listener which gets triggered on a Timer3-OutputCompareMatchB-Interrupt
	void setOCF3B(Listener* listener);
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF3C
	///Sets a Listener which gets triggered on a Timer3-OutputCompareMatchB-Interrupt
	void setOCF3C(Listener* listener);
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF3
	///Sets a Listener which gets triggered on a Timer3-OutputCompareMatchB-Interrupt
	void setICF3(Listener* listener);
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV4
	///Sets a Listener which gets triggered on a Timer4-Overflow-Interrupt
	void setTOV4(Listener* listener);
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF4A
	///Sets a Listener which gets triggered on a Timer4-OutputCompareMatchA-Interrupt
	void setOCF4A(Listener* listener);
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF4B
	///Sets a Listener which gets triggered on a Timer4-OutputCompareMatchB-Interrupt
	void setOCF4B(Listener* listener);
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF4C
	///Sets a Listener which gets triggered on a Timer4-OutputCompareMatchB-Interrupt
	void setOCF4C(Listener* listener);
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF4
	///Sets a Listener which gets triggered on a Timer4-OutputCompareMatchB-Interrupt
	void setICF4(Listener* listener);
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV5
	///Sets a Listener which gets triggered on a Timer5-Overflow-Interrupt
	void setTOV5(Listener* listener);
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF5A
	///Sets a Listener which gets triggered on a Timer5-OutputCompareMatchA-Interrupt
	void setOCF5A(Listener* listener);
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF5B
	///Sets a Listener which gets triggered on a Timer5-OutputCompareMatchB-Interrupt
	void setOCF5B(Listener* listener);
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF5C
	///Sets a Listener which gets triggered on a Timer5-OutputCompareMatchB-Interrupt
	void setOCF5C(Listener* listener);
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF5
	///Sets a Listener which gets triggered on a Timer5-OutputCompareMatchB-Interrupt
	void setICF5(Listener* listener);
	#endif
}


#endif /* ATMIGHTY_RESSOURCES_INTERRUPTS_ATMEGA2560_INTERRUPTMANAGER_H_ */
