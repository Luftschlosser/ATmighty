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

#include <ATmighty/Tools/Timing/Stopwatch/Stopwatch.h>


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
	AbstractHardwareManager abHw = AbstractHardwareManager(42);

	MessageLogWriter::Usart usbWriter;
	MessageLog<>::DefaultInstance().setWriter(&usbWriter);

	IrqTest blink(abHw.allocIoPin<'B',7>());

	Timer16bit* timer = abHw.allocTimer16bit<AbstractTimer3>();
	timer->setTimerOverflowISR(&blink);
	timer->enableTimerOverflowInterrupt(true);
	timer->setPrescalar(Timer16bit::Prescale::Scale8);
	sei();

	//Stopwatch-tests
	Stopwatch<AbstractTimer16bit> stopwatch = Stopwatch<AbstractTimer16bit>(abHw.allocTimer16bit<AbstractTimer4>());
	volatile uint32_t test;

	stopwatch.start();
	test = stopwatch.stop();
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "Stopwatch time 0: ", test);

	stopwatch.start();
	asm volatile ( "nop \n" );
	test = stopwatch.stop();
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "Stopwatch time 1: ", test);

	stopwatch.start();
	asm volatile ( "nop \n" );
	asm volatile ( "nop \n" );
	test = stopwatch.stop();
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "Stopwatch time 2: ", test);

	stopwatch.start();
	asm volatile ( "nop \n" );
	asm volatile ( "nop \n" );
	asm volatile ( "nop \n" );
	test = stopwatch.stop();
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "Stopwatch time 3: ", test);

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
	static uint8_t c(20);
	c++;
	if (c>=20)
	{
		c=0;
		blinky->toggle();
	}
}
