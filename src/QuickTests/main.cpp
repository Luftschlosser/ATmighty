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
#include <ATmighty/Tools/Timing/PeriodicTrigger/PeriodicTrigger.h>


IoPin* blinky;
AbstractHardwareManager abHw = AbstractHardwareManager(42);

void blink()
{
	static Stopwatch<AbstractTimer16bit> stopwatch = Stopwatch<AbstractTimer16bit>(abHw.allocTimer16bit<AbstractTimer1>());
	static volatile uint32_t test;

	blinky->toggle();
	if (!blinky->read())
	{
		stopwatch.start();
	}
	else
	{
		test = stopwatch.stop();
		MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "time between triggers: ", test);
	}
}

int main( void )
{
	MessageLogWriter::Usart usbWriter;
	MessageLog<>::DefaultInstance().setWriter(&usbWriter);

	blinky = abHw.allocIoPin<'B',7>();
	blinky->setDirection(IoPin::DataDirection::Output);

	sei();

	Timer16bit* timer = abHw.allocTimer16bit<AbstractTimer3>();
	PeriodicTrigger<> trigger = PeriodicTrigger<>(timer);

	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false,trigger.setPeriodHertz(887623));
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false,trigger.setPeriodHertz(1510));
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false,trigger.setPeriodHertz(7));
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false,trigger.setPeriodSeconds(7));
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false,trigger.setPeriodSeconds(2));
	trigger.setTriggerAction(&blink);

	trigger.start();

	//mainloop
	while(1){
		asm ( "nop \n" );
	}
}
