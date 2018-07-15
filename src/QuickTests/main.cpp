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

void blink()
{
	blinky->toggle();
}

int main( void )
{
	AbstractHardwareManager abHw = AbstractHardwareManager(42);

	MessageLogWriter::Usart usbWriter;
	MessageLog<>::DefaultInstance().setWriter(&usbWriter);

	blinky = abHw.allocIoPin<'B',7>();
	blinky->setDirection(IoPin::DataDirection::Output);

	sei();

	Timer16bit* timer = abHw.allocTimer16bit<AbstractTimer3>();
	PeriodicTrigger<> trigger = PeriodicTrigger<>(timer);
	trigger.setPeriod(16000000);
	trigger.setTriggerAction(&blink);
	trigger.start();


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

	//mainloop
	while(1){
		asm ( "nop \n" );
	}
}
