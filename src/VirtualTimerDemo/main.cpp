#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "ATmighty/Utilities/C++/FullCppSupport.h"
#include "ATmighty/Interfaces/Listener.h"
#include "ATmighty/Ressources/Periphery/Abstract/AbstractHardwareManager.h"
#include "ATmighty/Ressources/Periphery/Virtual/Timer/VirtualTimerPool.h"
#include "ATmighty/Utilities/Logs/MessageLog.h"
#include "ATmighty/Utilities/Logs/MessageLogWriter.h"
#include <ATmighty/Tools/Timing/PeriodicTrigger/PeriodicTrigger.h>


/*!
 * This example-project demonstrates the creation and usage of virtual Timers.
 * It also demonstrates a virtual Timer performing a PWM-output.
 */


//globally used variables
IoPin* blinky;
IoPin* virtualPwm;
uint8_t i = 0;
AbstractHardwareManager abHw(42);
VirtualTimer8bit* virtualTimer2;

void blink()
{
	//calculate new PWM-Level (to fade output)
	uint8_t j = (i / 4) + 1;
	if (j > (0xFF - i))
	{
		i = 0;
	}
	else
	{
		i += j;
	}

	//signal the sucessful call to "blink()" on another led
	blinky->toggle();

	//update PWM-outputlevel on the second virtual Timer
	virtualTimer2->setOCRx(i, 'A');
}

int main( void )
{
	MessageLogWriter::Usart usbWriter;
	MessageLog<>::DefaultInstance().setWriter(&usbWriter);

	//Pin setup
	virtualPwm = abHw.allocIoPin<'B',7>();
	blinky = abHw.allocIoPin<'B',2>();
	blinky->setDirection(IoPin::DataDirection::Output);

	//Enable Interrupts as this are needed for timer virtualization
	sei();

	//allocate (real) 16-bit Timer to be used for timer virtualization
	Timer16bit* abstractTimer = abHw.allocTimer16bit<AbstractTimer3>();

	//Initialize VirtualTimerPool: Run with a base-Frequency of 20KHz, use abstractTimer for virtualization, virtualize 2 Timers)
	VirtualTimerPool<> timerPool(20000, abstractTimer, 2);

	//Allocate first virtual Timer with 1 outputChannel (no Pins)
	VirtualTimer8bit* virtualTimer1 = timerPool.allocTimer8bit(1);

	//Use first virtual Timer to run a periodic trigger
	PeriodicTrigger<VirtualTimer8bit> trigger(virtualTimer1);

	//Allocate second virtual Timer with 1 outputChannel which is connected to pin "virtualPwm"
	virtualTimer2 = timerPool.allocTimer8bit(1, &virtualPwm);

	//Configure second virtual Timer to run in PWM-Mode (to drive Pin "virtualPwm")
	virtualTimer2->setWGM(3);
	virtualTimer2->setOCRx(i, 'A');
	virtualTimer2->setCOMx(2, 'A');
	virtualTimer2->setPrescalar(0);

	//start the virtual Timers
	timerPool.startAll();

	//configure PeriodicTrigger (which used the first virtual Timer) to trigger every 100ms and execute the "blink()"-function.
	trigger.setPeriodMilliseconds(100);
	trigger.setTriggerAction(&blink);
	trigger.start();

	//Do nothing more, setup complete -> Software will run asynchronously from now on.
	while(1)
	{
		asm ( "nop \n" );
	}
}
