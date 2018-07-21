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
IoPin* virtualPwm;
uint8_t i = 0;
AbstractHardwareManager abHw = AbstractHardwareManager(42);
VirtualTimer8bit* virtualTimer2;

void blink()
{
	i += (i / 12) + 1;
	blinky->toggle();
	virtualTimer2->setOCRx(i, 'A');
}

int main( void )
{
	MessageLogWriter::Usart usbWriter;
	MessageLog<>::DefaultInstance().setWriter(&usbWriter);

	virtualPwm = abHw.allocIoPin<'B',7>();
	blinky = abHw.allocIoPin<'B', 2>();
	blinky->setDirection(IoPin::DataDirection::Output);

	sei();

	Timer16bit* abstractTimer = abHw.allocTimer16bit<AbstractTimer3>();
	VirtualTimerPool<> timerPool = VirtualTimerPool<>(20000, abstractTimer, 2);

	VirtualTimer8bit* virtualTimer1 = timerPool.allocTimer8bit(1);
	PeriodicTrigger<VirtualTimer8bit> trigger = PeriodicTrigger<VirtualTimer8bit>(virtualTimer1);
	virtualTimer2 = timerPool.allocTimer8bit(2, &virtualPwm);

	virtualTimer2->setWGM(3);
	virtualTimer2->setOCRx(i, 'A');
	virtualTimer2->setCOMx(2, 'A');
	virtualTimer2->setPrescalar(0);

	timerPool.startAll();

	trigger.setPeriodMilliseconds(100);
	trigger.setTriggerAction(&blink);
	trigger.start();

	//mainloop
	while(1){
		asm ( "nop \n" );
	}
}
