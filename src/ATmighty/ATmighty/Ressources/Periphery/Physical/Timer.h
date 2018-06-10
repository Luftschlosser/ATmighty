/*!
 * This headerfile includes a collection of all timers for this µC.
 * This headerfile is µC-independent, it will include the µC-dependent timer-versions if available.
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_TIMER_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_TIMER_H_


#if defined (__AVR_ATmega2560__)
#  include "Timer/Atmega2560/Timer0.h"
#  include "Timer/Atmega2560/Timer2.h"
#else
#  warning "No Timers are defined for this µC."
#endif


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_TIMER_H_ */
