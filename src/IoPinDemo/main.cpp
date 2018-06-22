/*!
 * This program demonstrates the usage and the behavior of ATmighty's abstract IoPins.
 * It demonstrated allocation, freeing and usage of abstract IoPins, as well as
 * measuring the runtime costs of this operations.
 * PinB7 and B6 are used as the example pins, they get configured and used to make the internal
 * Arduino-LED (Pin B7) and any additional Led's on pin B6 blink with active delays within the mainloop.
 */

#include <stdlib.h>
#include "ATmighty/Utilities/C++/FullCppSupport.h"
#include "ATmighty/Ressources/Periphery/Abstract/AbstractHardwareManager.h"

#include "ATmighty/Utilities/Logs/MessageLog.h"
#include "ATmighty/Utilities/Logs/MessageLogWriter.h"
#include <ATmighty/Tools/Timing/Stopwatch/Stopwatch.h>


int main( void )
{
	//Instantiating AbstractHardwareManager instance as user ID 1
	AbstractHardwareManager hw(1);

	//Declaring abstract test hardware instances
	AbstractIoPin* pin;
	GeneralIoPin* anyPin;
	SpecificIoPin<'B', 7>* pinB7;
	AbstractIoPort* port;

	//Configuring default MessageLog
	MessageLogWriter::Usart usbWriter;
	MessageLog<>::DefaultInstance().setWriter(&usbWriter);

	//Configuring Stopwatch to measure Timings
	Stopwatch<> stopwatch(hw.allocTimer16bit<AbstractTimer1>());
	volatile uint32_t duration = 0;


	//__________________________________________
	//Testing AbstractHardwareManager Edge-Cases

	port = hw.allocIoPort<'B'>();	//Port B gets allocated (Port is specified at compile-time via template-parameter)
	pin = hw.allocIoPin<'B', 7>();	//IoPin B7 (specified by template-paramter) can't get allocated as the whole Port is in use. (pin == nullptr afterwards)
	hw.freeItem(&port);				//Port B gets freed (port == nullptr afterwards)
	hw.freeItem(&port);				//Port B is already free, freeing nullptr is impossible
	pin = hw.allocIoPin('B', 7);	//IoPin B7 gets allocated (Pin B7 is specified at runtime via method-arguments)
	port = hw.allocIoPort('B');		//Port B (specified by method-argument) can't get allocated as one of its pins is in use. (port == nullptr afterwards)
	anyPin = hw.allocIoPin('B', 7);	//IoPin B7 is already in use, allocation fails. (absPin == nullptr afterwards)
	hw.freeItem(&pin);				//freeing pin to cleanup before next demo-section.


	//________________________________________
	//Testing AbstractHardwareManager Runtimes

	stopwatch.start();
	pin = hw.allocIoPin<'B', 7>();
	duration = stopwatch.stop();
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "Abstract SpecificIoPin-allocation takes ", duration, " cycles.");

	hw.freeItem(&pin);

	stopwatch.start();
	pin = hw.allocIoPin('B', 7);
	duration = stopwatch.stop();
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "Abstract GeneralIoPin-allocation takes ", duration, " cycles.");

	stopwatch.start();
	hw.freeItem(&pin);
	duration = stopwatch.stop();
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "Abstract deallocation takes ", duration, " cycles.");


	//_________________________________
	//Testing size of different IoPins:

	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "Abstract hardware-item IoPin has size ", sizeof(IoPin));
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "Abstract hardware-item AbstractIoPin has size ", sizeof(AbstractIoPin));
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "Abstract hardware-item GeneralIoPin has size ", sizeof(GeneralIoPin));
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "Abstract hardware-item SpecificIoPin has size ", sizeof(SpecificIoPin<'B', 7>));


	//______________________________________
	//Testing abstract IoPin accessing times

	pinB7 = hw.allocIoPin<'B', 7>();
	anyPin = hw.allocIoPin('B', 6);
	pin = pinB7;

	stopwatch.start();
	pinB7->setDirection(IoPin::Input);
	duration = stopwatch.stop();
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "SpecificIoPin direct access takes ", duration, " cycles.");

	stopwatch.start();
	anyPin->setDirection(IoPin::Output);
	duration = stopwatch.stop();
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "GeneralIoPin direct access takes ", duration, " cycles.");

	stopwatch.start();
	pin->setDirection(IoPin::Output);
	duration = stopwatch.stop();
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "SpecificIoPin indirect access (via Interface-pointer) takes ", duration, " cycles.");


	//________
	//Mainloop

	pinB7->set(false);
	anyPin->set(true);

	while(1){
		for (uint32_t i = 0; i < 250000; i++)
		{
			asm volatile ( "nop \n" );
		}
		pinB7->toggle();
		anyPin->toggle();
	}
}
