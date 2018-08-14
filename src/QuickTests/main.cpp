#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "ATmighty/Utilities/C++/FullCppSupport.h"
#include "ATmighty/Interfaces/Listener.h"
#include "ATmighty/Ressources/Periphery/Abstract/AbstractHardwareManager.h"
#include "ATmighty/Ressources/Periphery/Abstract/IoPins.h"
#include "ATmighty/Utilities/Logs/MessageLog.h"
#include "ATmighty/Utilities/Logs/MessageLogWriter.h"
#include "ATmighty/Tools/Timing/TimedStatemachine/TimedStatemachine.h"
#include "ATmighty/Ressources/Periphery/Virtual/Timer/VirtualTimerPool.h"


AbstractHardwareManager abHw(42);
TimedStatemachine<Timer8bit> *ampel;
IoPin *red, *green, *yellow, *signalChange;


enum states : uint8_t
{
	Green,
	YellowToGreen,
	Red,
	YellowToRed
};

void blink()
{
	signalChange->set(true);
	for(uint16_t i = 1; i != 0; i++)
	{
		asm ( "nop \n" );
	}
	signalChange->set(false);
}

void greenOn()
{
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "green on");
	green->set(true);
}

void greenOff()
{
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "green off");
	green->set(false);
}

void redOn()
{
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "red on");
	red->set(true);
}

void redOff()
{
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "red off");
	red->set(false);
}

void yellowOn()
{
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "yellow on");
	yellow->set(true);
}

void yellowOff()
{
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "yellow off");
	yellow->set(false);
}

int main( void )
{
	MessageLogWriter::Usart usbWriter;
	MessageLog<>::DefaultInstance().setWriter(&usbWriter);

	//Timer Setup
	VirtualTimerPool<Timer16bit> vtPool(10, abHw.allocTimer16bit<AbstractTimer5>(), 1);
	VirtualTimer8bit *statemachineTimer = vtPool.allocTimer8bit(1);
	vtPool.startAll();

	//Pin setup
	signalChange = abHw.allocIoPin<'B',7>();
	signalChange->setDirection(IoPin::DataDirection::Output);
	red = abHw.allocIoPin<'L',1>();
	red->setDirection(IoPin::DataDirection::Output);
	yellow = abHw.allocIoPin<'L',3>();
	yellow->setDirection(IoPin::DataDirection::Output);
	green = abHw.allocIoPin<'L',5>();
	green->setDirection(IoPin::DataDirection::Output);

	sei();

	//Statemachine Setup
	ampel = new TimedStatemachine<Timer8bit>(4, statemachineTimer);
	ampel->setEnterAction(states::Green, &greenOn);
	ampel->setEnterAction(states::Red, &redOn);
	ampel->setEnterAction(states::YellowToGreen, &yellowOn);
	ampel->setEnterAction(states::YellowToRed, &yellowOn);
	ampel->setExitAction(states::Green, &greenOff);
	ampel->setExitAction(states::Red, &redOff);
	ampel->setExitAction(states::YellowToGreen, &yellowOff);
	ampel->setExitAction(states::YellowToRed, &yellowOff);
	ampel->setChangeAction(&blink);

	ampel->setTimedStatechange(states::Green, states::YellowToRed, 40);
	ampel->setTimedStatechange(states::YellowToRed, states::Red, 5);
	ampel->setTimedStatechange(states::Red, states::YellowToGreen, 50);
	ampel->setTimedStatechange(states::YellowToGreen, states::Green, 5);

	//TimeoutTrigger calibration
	ampel->calibrate();

	//start main
	ampel->start(states::Green);

	while(1)
	{
		asm ( "nop \n" );
	}
}
