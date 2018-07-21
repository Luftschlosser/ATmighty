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
#include "ATmighty/Ressources/Periphery/Virtual/Timer/VirtualTimerPool.h"

#include "ATmighty/Utilities/Logs/MessageLog.h"
#include "ATmighty/Utilities/Logs/MessageLogWriter.h"

#include <ATmighty/Tools/Timing/Stopwatch/Stopwatch.h>
#include <ATmighty/Tools/Timing/PeriodicTrigger/PeriodicTrigger.h>


IoPin* blinky;
AbstractHardwareManager abHw = AbstractHardwareManager(42);

void blink()
{
	MessageLog<>::DefaultInstance().log<LogLevel::Debug>(false, "Blink");

	blinky->toggle();
}

int main( void )
{
	MessageLogWriter::Usart usbWriter;
	MessageLog<>::DefaultInstance().setWriter(&usbWriter);

	blinky = abHw.allocIoPin<'B',7>();
	blinky->setDirection(IoPin::DataDirection::Output);

	sei();

	Timer16bit* abstractTimer = abHw.allocTimer16bit<AbstractTimer3>();
	VirtualTimerPool<> timerPool = VirtualTimerPool<>(100, abstractTimer, 2);
	VirtualTimer8bit* virtualTimer = timerPool.allocTimer8bit(1);
	PeriodicTrigger<VirtualTimer8bit> trigger = PeriodicTrigger<VirtualTimer8bit>(virtualTimer);
	timerPool.startAll();

	trigger.setPeriodSeconds(2);
	trigger.setTriggerAction(&blink);

	trigger.start();

	//mainloop
	while(1){
		asm ( "nop \n" );
	}
}
