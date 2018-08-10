#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "ATmighty/Utilities/C++/FullCppSupport.h"
#include "ATmighty/Interfaces/Listener.h"
#include "ATmighty/Ressources/Periphery/Abstract/AbstractHardwareManager.h"
#include "ATmighty/Ressources/Periphery/Abstract/IoPins.h"
#include "ATmighty/Utilities/Logs/MessageLog.h"
#include "ATmighty/Utilities/Logs/MessageLogWriter.h"
#include "ATmighty/Tools/Common/FiniteStatemachine/FiniteStatemachine.h"


AbstractHardwareManager abHw(41);
FiniteStatemachine ampel(4); //4 states in total
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
	uint32_t i = 0;

	signalChange->set(true);
	for(uint16_t i = 1; i != 0; i++)
	{
		asm ( "nop \n" );
	}
	signalChange->set(false);

	//state change
	switch(ampel.getState())
	{
	case states::Red:
		ampel.changeState(states::YellowToGreen);
		i = 30;
		break;
	case states::YellowToGreen:
		ampel.changeState(states::Green);
		i = 10;
		break;
	case states::Green:
		ampel.changeState(states::YellowToRed);
		i = 25;
		break;
	case states::YellowToRed:
		ampel.changeState(states::Red);
		i = 10;
		break;
	}

	//delay
	for(i *= 100000; i > 0; i--)
	{
		asm ( "nop \n" );
	}
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

	ampel.setEnterAction(states::Green, &greenOn);
	ampel.setEnterAction(states::Red, &redOn);
	ampel.setEnterAction(states::YellowToGreen, &yellowOn);
	ampel.setEnterAction(states::YellowToRed, &yellowOn);
	ampel.setExitAction(states::Green, &greenOff);
	ampel.setExitAction(states::Red, &redOff);
	ampel.setExitAction(states::YellowToGreen, &yellowOff);
	ampel.setExitAction(states::YellowToRed, &yellowOff);
	ampel.setChangeAction(&blink);

	ampel.start(states::Red);
	ampel.changeState(states::YellowToGreen);

	while(1)
	{
		asm ( "nop \n" );
	}
}
