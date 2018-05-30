/*!
 * This headerfile includes a collection of all abstract IO-Ports for this µC.
 * This headerfile is µC-independent, it will include the µC-dependent IoPort-versions if available.
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPORTS_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPORTS_H_


//base
#include "IoPorts/IoPort.h"

//specifics
#if defined (__AVR_ATmega2560__)
#  include "IoPorts/Atmega2560/AbstractPortA.h"
/*#  include "IoPorts/Atmega2560/PortB.h"
#  include "IoPorts/Atmega2560/PortC.h"
#  include "IoPorts/Atmega2560/PortD.h"
#  include "IoPorts/Atmega2560/PortE.h"
#  include "IoPorts/Atmega2560/PortF.h"*/
#else
#  warning "No abstract IoPorts are defined for this µC."
#endif


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPORTS_H_ */
