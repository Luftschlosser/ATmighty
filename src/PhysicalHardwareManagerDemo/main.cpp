/*!
 * This program demonstrates the usage and the behavior of ATmighty's PhysicalHardwareManager
 */

#include <stdlib.h>
#include "ATmighty/Utilities/C++/FullCppSupport.h"
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"
#include "ATmighty/Ressources/Periphery/Physical/Timer.h"
#include "ATmighty/Ressources/Periphery/Physical/Ports.h"

#include <avr/io.h>
#include "ATmighty/Ressources/Periphery/Abstract/AbstractHardwareManager.h"
#include "ATmighty/Utilities/Logs/MessageLog.h"
#include "ATmighty/Utilities/Logs/MessageLogWriter.h"
#include <ATmighty/Tools/Timing/Stopwatch/Stopwatch.h>


int main( void )
{
	//Setting namespace-alias for easier usage
	namespace Hw = PhysicalHardwareManager;

	//Declaring physical test-hardware instances
	Timer0 *timer, *testTimer;
	Port<'B'> *port;

	//Configuring default MessageLog
	MessageLogWriter::Usart usbWriter;
	MessageLog<>::DefaultInstance().setWriter(&usbWriter);

	//Configuring Stopwatch to measure Timings
	AbstractHardwareManager hw(0);
	Stopwatch<> stopwatch(hw.allocTimer16bit<AbstractTimer1>());
	volatile uint32_t duration = 0;


	//__________________________________________
	//Testing PhysicalHardwareManager Edge-Cases

	testTimer = Hw::Alloc<Timer0>(1);	//Normal Allocation of Timer0 by ID 1, testTimer will point to a real Timer0-instance afterwards.
	timer = Hw::Alloc<Timer0>(2);		//Allocation will fail, Timer0 is already in use, timer=nullptr afterwards.
	Hw::Free(&testTimer);				//Normal Deallocation, testTimer=nullptr afterwards.
	Hw::Free(&testTimer);				//Deallocation will fail, because testTimer=nullptr.
	timer = Hw::Alloc<Timer0>(2);		//Normal Allocation of Timer0 by ID 2.

	//________________________________________
	//Testing PhysicalHardwareManager Runtimes

	stopwatch.start();
	port = Hw::Alloc<Port<'B'>>(1);
	duration = stopwatch.stop();
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "Physical Allocation takes ", duration, " cycles.");

	stopwatch.start();
	Hw::Free(&timer);
	duration = stopwatch.stop();
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "Physical Deallocation takes ", duration, " cycles.");

	//_________________________________
	//Testing size of PhysicalHardware:

	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "Physical hardware-item Timer0 has size ", sizeof(Timer0));
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "Physical hardware-item Port<'B'> has size ", sizeof(Port<'B'>));


	//_________________________________________
	//Testing physical hardware accessing times

	stopwatch.start();
	port->setDDRx(0x88);
	duration = stopwatch.stop();
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "Physical hardware register-write takes ", duration, " cycles.");

	stopwatch.start();
	DDRB = 0xEF;
	duration = stopwatch.stop();
	MessageLog<>::DefaultInstance().log<LogLevel::Info>(false, "Direct register-write takes ", duration, " cycles.");

	port->setPORTx(0xFF);


	//________
	//Mainloop

	while(1){
		for (uint32_t i = 0; i < 250000; i++)
		{
			asm volatile ( "nop \n" );
		}
		//port->setPINx(0xEF); //For some reason, this gets stripped by compiler-optimization... [?]
		PINB = 0xEF; //But this works -> why? - it should compile to the exact same assembler!
	}
}
