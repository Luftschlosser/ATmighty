/*
 * main.cpp
 *
 *  Created on: 31.03.2018
 *      Author: valentin
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h>
#include "ATmighty/Ressources/Periphery/Physical/Timer.h"

uint8_t c = 0;
uint8_t x = 0;

int main( void )
{
	unsigned long i = 0;

	//TestArea
	PhysicalHardwareManager ph = PhysicalHardwareManager();
	Timer0* timer = ph.allocHardware<Timer0>(1);

	DDRB |= _BV(PB7); // PortPB7 als Output konfigurieren

	//serial0 init
	UBRR0H = (0x0F & (uint8_t)(103>>8));
	UBRR0L = (uint8_t)(103);
	UCSR0C = (0<<4)|(((1-1)&0x1)<<3)|(((8-5)&0x3)<<1);
	UCSR0B |= (1<<TXEN0);  //enable Transmit-Unit

	timer->setTCCR0B(5);//set prescalar 1024
	timer->setTIMSK0(1);//enable overflow-interrupt
	sei();

	while(1){
		PORTB &= ~_BV(PB7);
		for (i = 250000; i>0; i--){
			asm ( "nop \n" );
		}
		PORTB |= _BV(PB7);
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

