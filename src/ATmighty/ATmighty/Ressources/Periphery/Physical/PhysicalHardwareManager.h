/*!
 * This class manages the exclusive direct access to the physical periphery of this µC.
 * This headerfile is µC-independent, it will include the µC-dependent version if available.
 */


#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_PHYSICALHARDWAREMANAGER_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_PHYSICALHARDWAREMANAGER_H_


#if defined (__AVR_ATmega2560__)
#  include "PhysicalHardwareManager/Atmega2560/PhysicalHardwareManager.h"
#else
#  warning "PhysicalHardwareManager is not defined for this µC."
#endif


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_PHYSICALHARDWAREMANAGER_H_ */
