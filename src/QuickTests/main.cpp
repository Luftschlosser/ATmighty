/*
 * main.cpp
 *
 *  Created on: 31.03.2018
 *      Author: valentin
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <ATmighty/Ressources/Periphery/Physical/Ports.h>
#include <stdlib.h>

#include "ATmighty/Utilities/C++/FullCppSupport.h"

#include "ATmighty/Ressources/Periphery/Physical/Timer.h"

#include "ATmighty/Ressources/Periphery/Abstract/AbstractHardwareManager.h"
#include "ATmighty/Ressources/Periphery/Abstract/IoPorts.h"
#include "ATmighty/Ressources/Periphery/Abstract/IoPins.h"

#include "ATmighty/Utilities/Logs/MessageLog.h"
#include "ATmighty/Utilities/Logs/MessageLogWriter.h"

AbstractIoPin* absPin;
AbstractIoPort* absPort;

int main( void )
{
	namespace phHw = PhysicalHardwareManager;
	AbstractHardwareManager abHw = AbstractHardwareManager(42);

	MessageLogWriter::Usart usbWriter;
	MessageLog<>::DefaultInstance().setWriter(&usbWriter);

	Timer0* timer = phHw::Alloc<Timer0>(0);
	absPin = abHw.allocIoPin<'A',1>();
	abHw.freeItem(&absPin);
	absPin = abHw.allocIoPin<'L',5>();
	absPort = abHw.allocIoPort<'A'>();

	//Pin setup
	absPin->setDirection(IoPin::DataDirection::Output);
	absPin->set(true);

	//Port setup
	absPort->setDataDirectionMask(0xff);
	absPort->setData(0xff);

	//Timer setup
	timer->setTCCR0B(5);//set prescalar 1024
	timer->setTIMSK0(1);//enable overflow-interrupt
	sei();

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
		absPin->toggle();
		absPort->applyPinToggleMask(0x0f);
	}
}
