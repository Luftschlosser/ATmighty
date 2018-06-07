/*!
 * This headerfile includes a collection of all physical IO-Ports for this µC.
 * This headerfile is µC-independent, it will include the µC-dependent IoPort-versions if available.
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_PORTS_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_PORTS_H_


#if defined (__AVR_ATmega2560__)
#  include <ATmighty/Ressources/Periphery/Physical/Ports/Atmega2560/Port.h>
#else
#  warning "No physical IoPorts are defined for this µC."
#endif


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_PORTS_H_ */
