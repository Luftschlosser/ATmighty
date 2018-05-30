/*!
 * This class manages the exclusive access to the abstract periphery of this µC.
 * This headerfile is µC-independent, it will include the µC-dependent version if available.
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_ABSTRACTHARDWAREMANAGER_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_ABSTRACTHARDWAREMANAGER_H_


#if defined (__AVR_ATmega2560__)
#  include "AbstractHardwareManager/Atmega2560/AbstractHardwareManager.h"
#else
#  warning "AbstractHardwareManager is not defined for this µC."
#endif


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_ABSTRACTHARDWAREMANAGER_H_ */
