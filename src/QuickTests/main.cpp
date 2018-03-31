/*
 * main.cpp
 *
 *  Created on: 31.03.2018
 *      Author: valentin
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <test.h>

int main( void )
{
	unsigned long i = 0;
	uint8_t x = 0;


	DDRB |= _BV(PB7); // PortPB7 als Output konfigurieren

	//serial0 init
	UBRR0H = (0x0F & (uint8_t)(103>>8));
	UBRR0L = (uint8_t)(103);
	UCSR0C = (0<<4)|(((1-1)&0x1)<<3)|(((8-5)&0x3)<<1);
	UCSR0B |= (1<<TXEN0);  //enable Transmit-Unit

	while(1){
		PORTB &= ~_BV(PB7);
		for (i = 250000; i>0; i--){
			asm ( "nop \n" );
		}
		PORTB |= _BV(PB7);
		for (i = 250000; i>0; i--){
			asm ( "nop \n" );
		}

		x = test(x);
	}
}

