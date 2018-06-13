/*!
 * This headerfile includes a namespace with ISR-routines and dynamic Listener-instances for each interrupt source.
 * This headerfile is µC-independent, it will include the µC-dependent InterruptManager-versions if available.
 */

#ifndef ATMIGHTY_RESSOURCES_INTERRUPTS_INTERRUPTMANAGER_H_
#define ATMIGHTY_RESSOURCES_INTERRUPTS_INTERRUPTMANAGER_H_


#if defined (__AVR_ATmega2560__)
#  include "Atmega2560/InterruptManager.h"
#else
#  warning "No InterruptManager is defined for this µC."
#endif


#endif /* ATMIGHTY_RESSOURCES_INTERRUPTS_INTERRUPTMANAGER_H_ */
