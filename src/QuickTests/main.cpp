#include <avr/io.h>
#include <stdlib.h>

#include "ATmighty/Utilities/C++/FullCppSupport.h"
#include "ATmighty/Ressources/Periphery/Abstract/AbstractHardwareManager.h"
#include "ATmighty/Tools/Timing/Stopwatch/Stopwatch.h"
#include "ATmighty/Utilities/Logs/MessageLog.h"
#include "ATmighty/Utilities/Logs/MessageLogWriter.h"


/*
 * This project currently implements a setup to test for ram-usage
 */


//Global Variables
AbstractHardwareManager abHw(42);


//return the amount of currently free ram in bytes
int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

//returns the current size of the stack in bytes
int stackSize(){
	int v;
	return 8703 - (int)&v;
}

//returns the size of the statically used ram in bytes
int staticSize(){
	extern int __heap_start;
	return (int)&__heap_start - 0x1FF;
}

//returns the current size of the dynamically allocated ram
int heapSize(){
	extern int __heap_start, *__brkval;
	return (__brkval == 0 ? 0 : (int)__brkval - (int)&__heap_start);
}

//The test-routine without anything to test
int main( void )
{
	MessageLogWriter::Usart usbWriter;
	MessageLog<>::DefaultInstance().setWriter(&usbWriter);
	sei();

	//Set up Test-environment here

	MessageLog<>::DefaultInstance().log<LogLevel::Fatal>(false, "Static: ", staticSize(), ",  Stack: ", stackSize(), ",  Heap: ", heapSize(), ",  Free: ", freeRam());
	while(1)
	{
		asm ("nop\n");
	}
}
