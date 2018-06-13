/*
 * main.cpp
 *
 *  Created on: 31.03.2018
 *      Author: valentin
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "ATmighty/Utilities/C++/FullCppSupport.h"
#include "ATmighty/Interfaces/Listener.h"

#include "ATmighty/Ressources/Periphery/Abstract/AbstractHardwareManager.h"

#include "ATmighty/Utilities/Logs/MessageLog.h"
#include "ATmighty/Utilities/Logs/MessageLogWriter.h"


class IrqTest : public Listener
{
	private:
		IoPin* blinky;

	public:
		IrqTest(IoPin* pin);
		void trigger();
};


int main( void )
{
	AbstractHardwareManager abHw = AbstractHardwareManager(41);

	MessageLogWriter::Usart usbWriter;
	MessageLog<>::DefaultInstance().setWriter(&usbWriter);

	IrqTest blink(abHw.allocIoPin<'B',7>());

	Timer8bit* timer = abHw.allocTimer8bit<AbstractTimer8bit>();
	timer->setTimerOverflowISR(&blink);
	timer->enableTimerOverflowInterrupt(true);
	timer->setPrescalar(Timer8bit::Prescale::Scale1024);
	sei();

	//mainloop
	while(1){
		asm ( "nop \n" );
	}
}

IrqTest::IrqTest(IoPin* pin) : blinky(pin)
{
	pin->setDirection(IoPin::DataDirection::Output);
}

void IrqTest::trigger()
{
	static uint8_t c(41);
	c++;
	if (c>=42)
	{
		c=0;
		blinky->toggle();
	}
}
