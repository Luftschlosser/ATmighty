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
#include "ATmighty/Ressources/Periphery/Physical/Usart.h"
#include "ATmighty/Utilities/Logs/MessageLog.h"
#include "ATmighty/Utilities/Logs/MessageLogWriter.h"


uint8_t c = 0;

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
	namespace hw = PhysicalHardwareManager;

	unsigned long i = 0;
	MessageLogWriter::Usart usbWriter;

	Timer0* timer = hw::Alloc<Timer0>(1);
	PortA* port = hw::Alloc<PortA>(1);

	//Pin A1 setup
	port->setDDRA(1);
	port->setPORTA(1);

	test<false>("Hallo Welt");

	//Timer setup
	timer->setTCCR0B(5);//set prescalar 1024
	timer->setTIMSK0(1);//enable overflow-interrupt
	sei();

	for (long i = 5000000; i>0; i--){
		asm ( "nop \n" );
	}
	MessageLog<>::DefaultInstance().setWriter(&usbWriter);

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
	}
}
