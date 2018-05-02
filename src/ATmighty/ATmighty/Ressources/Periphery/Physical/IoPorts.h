/*!
 * This headerfile includes a collection of all IO-Ports for this µC.
 * This headerfile is µC-independent, it will include the µC-dependent IoPort-versions if available.
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_IOPORTS_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_IOPORTS_H_


#if defined (__AVR_ATmega2560__)
#  include "IoPorts/Atmega2560/PortA.h"
#else
#  warning "No IoPorts are defined for this µC."
#endif


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_IOPORTS_H_ */
