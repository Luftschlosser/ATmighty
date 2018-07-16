/*!
 * This headerfile includes a namespace with ISR-routines and dynamic ISR-instances for each interrupt source of the atmega2560.
 * This headerfile is µC-dependent, it should only be included via the general InterruptManager.h-file.
 */

#ifndef ATMIGHTY_RESSOURCES_INTERRUPTS_ATMEGA2560_INTERRUPTMANAGER_H_
#define ATMIGHTY_RESSOURCES_INTERRUPTS_ATMEGA2560_INTERRUPTMANAGER_H_


#include "Config/InterruptConfig.h"


//forward declaration of "Listener"
class Listener;

/*!
 * This namespace includes functions to set Listeners/Callback functions for each managed Interrupt-source.
 * Which Interrupts get managed by this InterruptManager can be configured in "InterruptConfig.h"
 */
namespace InterruptManager
{
	/*!
	 * Returns true when called from inside a blocking ISR-Routine (nested interrupts currently disabled)
	 * This function can be useful inside Listeners and callback-routines which might not know whether they are
	 * executed from normal code or from an ISR. (For example a long callback-handler might want to enable
	 * nested interrupts to not block any other operations, but only if he is called from an ISR)
	 */
	bool isInBlockingIsr();

	/*!
	 * Re-enables interrupts when called from inside a blocking ISR.
	 * This can be useful for long callback-routines which would otherwise block interrupts for a long time.
	 * Keep in mind that recursive calls to ISR's will crash the processor with an stack-overflow!
	 */
	void enableNesting();

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV0
	///Sets a Listener which gets triggered on a Timer0-Overflow-Interrupt
	void setTOV0(Listener* listener);
	///Sets a pointer to a callback-function which gets called on a Timer0-Overflow-Interrupt
	void setTOV0(void (*callback)());
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF0A
	///Sets a Listener which gets triggered on a Timer0-OutputCompareMatchA-Interrupt
	void setOCF0A(Listener* listener);
	///Sets a pointer to a callback-function which gets called on a Timer0-OutputCompareMatchA-Interrupt
	void setOCF0A(void (*callback)());
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF0B
	///Sets a Listener which gets triggered on a Timer0-OutputCompareMatchB-Interrupt
	void setOCF0B(Listener* listener);
	///Sets a pointer to a callback-function which gets called on a Timer0-OutputCompareMatchB-Interrupt
	void setOCF0B(void (*callback)());
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV1
	///Sets a Listener which gets triggered on a Timer1-Overflow-Interrupt
	void setTOV1(Listener* listener);
	///Sets a pointer to a callback-function which gets called on a Timer1-Overflow-Interrupt
	void setTOV1(void (*callback)());
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF1A
	///Sets a Listener which gets triggered on a Timer1-OutputCompareMatchA-Interrupt
	void setOCF1A(Listener* listener);
	///Sets a pointer to a callback-function which gets called on a Timer1-OutputCompareMatchA-Interrupt
	void setOCF1A(void (*callback)());
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF1B
	///Sets a Listener which gets triggered on a Timer1-OutputCompareMatchB-Interrupt
	void setOCF1B(Listener* listener);
	///Sets a pointer to a callback-function which gets called on a Timer1-OutputCompareMatchB-Interrupt
	void setOCF1B(void (*callback)());
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF1C
	///Sets a Listener which gets triggered on a Timer1-OutputCompareMatchB-Interrupt
	void setOCF1C(Listener* listener);
	///Sets a pointer to a callback-function which gets called on a Timer1-OutputCompareMatchB-Interrupt
	void setOCF1C(void (*callback)());
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF1
	///Sets a Listener which gets triggered on a Timer1-OutputCompareMatchB-Interrupt
	void setICF1(Listener* listener);
	///Sets a pointer to a callback-function which gets called on a Timer1-OutputCompareMatchB-Interrupt
	void setICF1(void (*callback)());
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV2
	///Sets a Listener which gets triggered on a Timer2-Overflow-Interrupt
	void setTOV2(Listener* listener);
	///Sets a pointer to a callback-function which gets called on a Timer2-Overflow-Interrupt
	void setTOV2(void (*callback)());
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF2A
	///Sets a Listener which gets triggered on a Timer2-OutputCompareMatchA-Interrupt
	void setOCF2A(Listener* listener);
	///Sets a pointer to a callback-function which gets called on a Timer2-OutputCompareMatchA-Interrupt
	void setOCF2A(void (*callback)());
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF2B
	///Sets a Listener which gets triggered on a Timer2-OutputCompareMatchB-Interrupt
	void setOCF2B(Listener* listener);
	///Sets a pointer to a callback-function which gets called on a Timer2-OutputCompareMatchB-Interrupt
	void setOCF2B(void (*callback)());
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV3
	///Sets a Listener which gets triggered on a Timer3-Overflow-Interrupt
	void setTOV3(Listener* listener);
	///Sets a pointer to a callback-function which gets called on a Timer3-Overflow-Interrupt
	void setTOV3(void (*callback)());
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF3A
	///Sets a Listener which gets triggered on a Timer3-OutputCompareMatchA-Interrupt
	void setOCF3A(Listener* listener);
	///Sets a pointer to a callback-function which gets called on a Timer3-OutputCompareMatchA-Interrupt
	void setOCF3A(void (*callback)());
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF3B
	///Sets a Listener which gets triggered on a Timer3-OutputCompareMatchB-Interrupt
	void setOCF3B(Listener* listener);
	///Sets a pointer to a callback-function which gets called on a Timer3-OutputCompareMatchB-Interrupt
	void setOCF3B(void (*callback)());
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF3C
	///Sets a Listener which gets triggered on a Timer3-OutputCompareMatchB-Interrupt
	void setOCF3C(Listener* listener);
	///Sets a pointer to a callback-function which gets called on a Timer3-OutputCompareMatchB-Interrupt
	void setOCF3C(void (*callback)());
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF3
	///Sets a Listener which gets triggered on a Timer3-OutputCompareMatchB-Interrupt
	void setICF3(Listener* listener);
	///Sets a pointer to a callback-function which gets called on a Timer3-OutputCompareMatchB-Interrupt
	void setICF3(void (*callback)());
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV4
	///Sets a Listener which gets triggered on a Timer4-Overflow-Interrupt
	void setTOV4(Listener* listener);
	///Sets a pointer to a callback-function which gets called on a Timer4-Overflow-Interrupt
	void setTOV4(void (*callback)());
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF4A
	///Sets a Listener which gets triggered on a Timer4-OutputCompareMatchA-Interrupt
	void setOCF4A(Listener* listener);
	///Sets a pointer to a callback-function which gets called on a Timer4-OutputCompareMatchA-Interrupt
	void setOCF4A(void (*callback)());
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF4B
	///Sets a Listener which gets triggered on a Timer4-OutputCompareMatchB-Interrupt
	void setOCF4B(Listener* listener);
	///Sets a pointer to a callback-function which gets called on a Timer4-OutputCompareMatchB-Interrupt
	void setOCF4B(void (*callback)());
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF4C
	///Sets a Listener which gets triggered on a Timer4-OutputCompareMatchB-Interrupt
	void setOCF4C(Listener* listener);
	///Sets a pointer to a callback-function which gets called on a Timer4-OutputCompareMatchB-Interrupt
	void setOCF4C(void (*callback)());
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF4
	///Sets a Listener which gets triggered on a Timer4-OutputCompareMatchB-Interrupt
	void setICF4(Listener* listener);
	///Sets a pointer to a callback-function which gets called on a Timer4-OutputCompareMatchB-Interrupt
	void setICF4(void (*callback)());
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV5
	///Sets a Listener which gets triggered on a Timer5-Overflow-Interrupt
	void setTOV5(Listener* listener);
	///Sets a pointer to a callback-function which gets called on a Timer5-Overflow-Interrupt
	void setTOV5(void (*callback)());
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF5A
	///Sets a Listener which gets triggered on a Timer5-OutputCompareMatchA-Interrupt
	void setOCF5A(Listener* listener);
	///Sets a pointer to a callback-function which gets called on a Timer5-OutputCompareMatchA-Interrupt
	void setOCF5A(void (*callback)());
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF5B
	///Sets a Listener which gets triggered on a Timer5-OutputCompareMatchB-Interrupt
	void setOCF5B(Listener* listener);
	///Sets a pointer to a callback-function which gets called on a Timer5-OutputCompareMatchB-Interrupt
	void setOCF5B(void (*callback)());
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF5C
	///Sets a Listener which gets triggered on a Timer5-OutputCompareMatchB-Interrupt
	void setOCF5C(Listener* listener);
	///Sets a pointer to a callback-function which gets called on a Timer5-OutputCompareMatchB-Interrupt
	void setOCF5C(void (*callback)());
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF5
	///Sets a Listener which gets triggered on a Timer5-OutputCompareMatchB-Interrupt
	void setICF5(Listener* listener);
	///Sets a pointer to a callback-function which gets called on a Timer5-OutputCompareMatchB-Interrupt
	void setICF5(void (*callback)());
	#endif
}


#endif /* ATMIGHTY_RESSOURCES_INTERRUPTS_ATMEGA2560_INTERRUPTMANAGER_H_ */
