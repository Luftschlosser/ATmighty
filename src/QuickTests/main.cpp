/*
 * main.cpp
 *
 *  Created on: 31.03.2018
 *      Author: valentin
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h>
#include "ATmighty/Ressources/Periphery/Physical/IoPorts.h"
#include "ATmighty/Ressources/Periphery/Physical/Timer.h"
#include "ATmighty/Ressources/Periphery/Physical/Usart.h"

uint8_t c = 0;
uint8_t x = 0;

int main( void )
{
	unsigned long i = 0;
	PhysicalHardwareManager ph = PhysicalHardwareManager();

	Timer0* timer = ph.allocHardware<Timer0>(1);
	PortA* port = ph.allocHardware<PortA>(1);
	Usart0* usb = ph.allocHardware<Usart0>(1);


	//Serial init
	usb->setUBRR0H(0x0F & (uint8_t)(103>>8));
	usb->setUBRR0L((uint8_t)(103));
	usb->setUCSR0C((0<<4)|(((1-1)&0x1)<<3)|(((8-5)&0x3)<<1));
	usb->setUCSR0B(usb->getUCSR0B() | (1<<TXEN0));  //enable Transmit-Unit

	//Pin A1 setup
	port->setDDRA(1);
	port->setPORTA(1);

	//Timer setup
	timer->setTCCR0B(5);//set prescalar 1024
	timer->setTIMSK0(1);//enable overflow-interrupt
	sei();

	//mainloop
	while(1){
		port->setPINA(1); //toggle pin A1
		for (i = 250000; i>0; i--){
			asm ( "nop \n" );
		}
	}
}

ISR(TIMER0_OVF_vect)
{
	c++;
	if (c>=24)
	{
		c=0;
		UDR0 = ('A' + sizeof(Timer0));
	}
}

