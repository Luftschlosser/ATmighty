/*!
 * This headerfile includes the correct PortData for this µC.
 * This headerfile is µC-independent, it will include the µC-dependent PortData-versions if available.
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_UTILITIES_IOPORTADDRESSES_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_UTILITIES_IOPORTADDRESSES_H_


//µC-dependent port-data
#if defined (__AVR_ATmega2560__)
#  include <ATmighty/Ressources/Periphery/Utilities/IoPortAddresses/Atmega2560/IoPortAddresses.h>
#else
#  warning "No PortData defined for this µC."
#endif


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_UTILITIES_IOPORTADDRESSES_H_ */
