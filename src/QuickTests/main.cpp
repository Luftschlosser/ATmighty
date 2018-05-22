/*
 * main.cpp
 *
 *  Created on: 31.03.2018
 *      Author: valentin
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "ATmighty/Ressources/Periphery/Physical/IoPorts.h"
#include "ATmighty/Ressources/Periphery/Physical/Timer.h"
#include "ATmighty/Utilities/Logs/MessageLog.h"
#include "ATmighty/Utilities/Logs/MessageLogWriter.h"


void * operator new(size_t n)
{
	if (n == 0){n=1;}
	///if malloc return 0 -> reset avr + fatal error
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


PortA* port;

int main( void )
{
	namespace hw = PhysicalHardwareManager;

	MessageLogWriter::Usart usbWriter;
	MessageLog<>::DefaultInstance().setWriter(&usbWriter);

	Timer0* timer = hw::Alloc<Timer0>(0);
	port = hw::Alloc<PortA>(1);

	//Pin A1 setup
	port->setDDRA(1);
	port->setPORTA(1);

	//Timer setup
	timer->setTCCR0B(5);//set prescalar 1024
	timer->setTIMSK0(1);//enable overflow-interrupt
	sei();

	hw::Free<Timer0>(&timer);
	hw::Free<Timer0>(&timer);
	timer = hw::Alloc<Timer0>(2);
	timer = hw::Alloc<Timer0>(3);

	//mainloop
	while(1){
		asm ( "nop \n" );
	}
}

ISR(TIMER0_OVF_vect)
{
	static uint8_t c(41);
	c++;
	if (c>=42)
	{
		c=0;
		port->setPINA(1);
	}
}
