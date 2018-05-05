/*
 * main.cpp
 *
 *  Created on: 31.03.2018
 *      Author: valentin
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "ATmighty/Ressources/Periphery/Physical/IoPorts.h"
#include "ATmighty/Ressources/Periphery/Physical/Timer.h"
#include "ATmighty/Ressources/Periphery/Physical/Usart.h"
#include "ATmighty/Utilities/Logs/MessageLog.h"
#include "ATmighty/Utilities/Logs/MessageLogWriter.h"


void * operator new(size_t n)
{
	if (n == 0){n=1;}
  return malloc(n);
}

void operator delete(void * p) // or delete(void *, std::size_t)
{
  free(p);
}

//src: https://www.avrfreaks.net/forum/avr-c-micro-how?page=all
__extension__ typedef int __guard __attribute__((mode (__DI__)));
extern "C" int __cxa_guard_acquire(__guard *g){return !*(char *)(g);}
extern "C" void __cxa_guard_release (__guard *g){*(char *)g = 1;}
extern "C" void __cxa_guard_abort (__guard *){}
extern "C" void __cxa_pure_virtual(void){}
extern "C" void	atexit( void ) { }//MCU would never "exit", so atexit can be dummy.

int main( void )
{
	namespace hw = PhysicalHardwareManager;
	MessageLogWriter::Usart usbWriter;
	Timer0* timer = hw::Alloc<Timer0>(37);
	PortA* port = hw::Alloc<PortA>(-111);

	MessageLog<>::DefaultInstance().setWriter(&usbWriter);

	static const char test[] PROGMEM = "Programspace!!!";

	MessageLog<>::DefaultInstance().log<LogLevel::Debug>(test, true);

	//Pin A1 setup
	port->setDDRA(1);
	port->setPORTA(1);

	//Timer setup
	timer->setTCCR0B(5);//set prescalar 1024
	timer->setTIMSK0(1);//enable overflow-interrupt
	sei();

	hw::Free<Timer0>(&timer);
	hw::Free<Timer0>(&timer);
	timer = hw::Alloc<Timer0>(8);

	//mainloop
	while(1){
		asm ( "nop \n" );
	}
}

ISR(TIMER0_OVF_vect)
{
	static uint8_t c(0);
	c++;
	if (c>=42)
	{
		c=0;
		PINA=1;
	}
}
