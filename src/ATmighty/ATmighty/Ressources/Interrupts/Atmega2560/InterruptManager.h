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
}


#endif /* ATMIGHTY_RESSOURCES_INTERRUPTS_ATMEGA2560_INTERRUPTMANAGER_H_ */
