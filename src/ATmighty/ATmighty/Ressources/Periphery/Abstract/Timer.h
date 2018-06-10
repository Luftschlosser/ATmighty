/*!
 * This headerfile includes a collection of all abstract Timers for this µC.
 * This headerfile is µC-independent, it will include the µC-dependent Timer-versions if available.
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_TIMER_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_TIMER_H_


//bases
#include <ATmighty/Ressources/Periphery/Abstract/Timer/Timer8bit.h>
#include <ATmighty/Ressources/Periphery/Abstract/Timer/AbstractTimer8bit.h>

//specifics
#if defined (__AVR_ATmega2560__)
#  include "Timer/Atmega2560/AbstractTimer0.h"
#else
#  warning "No abstract Timers are defined for this µC."
#endif


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_TIMER_H_ */
