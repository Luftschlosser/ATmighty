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
IoPin *emergencyButton;


enum states : uint8_t
{
	Green,
	Yellow2Green,
	Red,
	Yellow2Red
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
	emergencyButton = abHw.allocIoPin<'L',7>();
	emergencyButton->setDirection(IoPin::DataDirection::Input);

	//Enable global Interrupts
	sei();

	//Statemachine Setup
	ampel = new TimedStatemachine<Timer8bit>(4, statemachineTimer);
	ampel->setEnterAction(states::Green, &greenOn);
	ampel->setEnterAction(states::Red, &redOn);
	ampel->setEnterAction(states::Yellow2Green, &yellowOn);
	ampel->setEnterAction(states::Yellow2Red, &yellowOn);
	ampel->setExitAction(states::Green, &greenOff);
	ampel->setExitAction(states::Red, &redOff);
	ampel->setExitAction(states::Yellow2Green, &yellowOff);
	ampel->setExitAction(states::Yellow2Red, &yellowOff);
	ampel->setChangeAction(&blink);

	ampel->setTimedStatechange(states::Green, states::Yellow2Red, 20);
	ampel->setTimedStatechange(states::Yellow2Red, states::Red, 10);
	ampel->setTimedStatechange(states::Red, states::Yellow2Green, 30);
	ampel->setTimedStatechange(states::Yellow2Green, states::Green, 10);

	//start main
	ampel->start(states::Green);

	bool val = false;
	while(1)
	{
		bool buf = emergencyButton->read();
		if (!buf && val && (ampel->getState() == states::Red))
		{
			MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "Instant Green!");
			ampel->changeState(states::Yellow2Green);
		}
		val = buf;
	}
}
