/*!
 * This headerfile includes a collection of all USART-Hardware for this µC.
 * This headerfile is µC-independent, it will include the µC-dependent Usart-versions if available.
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_USART_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_USART_H_


#if defined (__AVR_ATmega2560__)
#  include "Usart/Atmega2560/Usart0.h"
#else
#  warning "No USARTs are defined for this µC."
#endif


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_USART_H_ */
