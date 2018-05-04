/*
 * main.cpp
 *
 *  Created on: 31.03.2018
 *      Author: valentin
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h>
#include "ATmighty/DataStructures/IoQueue.h"
#include "ATmighty/Utilities/Logs/MessageLog.h"

uint8_t c = 0;
MessageLog<LogLevel::Info> log(100);

void * operator new(size_t n)
{
  void * const p = malloc(n);
  // handle p == 0
  return p;
}

void operator delete(void * p) // or delete(void *, std::size_t)
{
  free(p);
}

template <bool debug>
void test(const char* txt)
{
	if (debug)
	{
		while (*txt)
		{
			UDR0 = *txt;
			txt++;
			for (long i = 250000; i>0; i--){
				asm ( "nop \n" );
			}
		}
		return;
	}
}

int main( void )
{
	unsigned long i = 0;

	MessageLogWriter::Usart usbWriter;
	PhysicalHardwareManager ph;

	Timer0* timer = ph.alloc<Timer0>(1);
	PortA* port = ph.alloc<PortA>(1);

	//Pin A1 setup
	port->setDDRA(1);
	port->setPORTA(1);

	test<false>("Hallo Welt");

	//Timer setup
	timer->setTCCR0B(5);//set prescalar 1024
	timer->setTIMSK0(1);//enable overflow-interrupt
	sei();

	log.debug("Initedededed! :P");//log init complete
	for (long i = 5000000; i>0; i--){
		asm ( "nop \n" );
	}
	log.setWriter(usbWriter);

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
	if (c>=50)
	{
		c=0;
		log.info("Schnurz-Piepe!");
	}
}

