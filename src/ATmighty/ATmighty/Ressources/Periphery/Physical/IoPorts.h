/*!
 * This headerfile includes a collection of all physical IO-Ports for this µC.
 * This headerfile is µC-independent, it will include the µC-dependent IoPort-versions if available.
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_IOPORTS_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_IOPORTS_H_


#if defined (__AVR_ATmega2560__)
#  include "IoPorts/Atmega2560/PortA.h"
#  include "IoPorts/Atmega2560/PortB.h"
#  include "IoPorts/Atmega2560/PortC.h"
#  include "IoPorts/Atmega2560/PortD.h"
#  include "IoPorts/Atmega2560/PortE.h"
#  include "IoPorts/Atmega2560/PortF.h"
#else
#  warning "No physical IoPorts are defined for this µC."
#endif


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_IOPORTS_H_ */
