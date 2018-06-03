/*!
 * This headerfile includes a collection of all abstract IO-Pins for this µC.
 * This headerfile is µC-independent, it will include the µC-dependent IoPin-versions if available.
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPINS_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPINS_H_


//bases
#include <ATmighty/Ressources/Periphery/Abstract/IoPins/IoPin.h>
#include <ATmighty/Ressources/Periphery/Abstract/IoPins/AbstractIoPin.h>

//specifics
#if defined (__AVR_ATmega2560__)
#  include "IoPins/Atmega2560/AbstractPinA0.h"
#else
#  warning "No abstract IoPorts are defined for this µC."
#endif


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPINS_H_ */
