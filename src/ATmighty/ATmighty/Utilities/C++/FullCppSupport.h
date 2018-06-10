/*!
 * This headerfile contains global functions to enable full c++-Support on AVR's
 */

#ifndef ATMIGHTY_UTILITIES_C___FULLCPPSUPPORT_H_
#define ATMIGHTY_UTILITIES_C___FULLCPPSUPPORT_H_


#include <stdlib.h>


void * operator new(size_t n)
{
	if (n == 0)
	{
		n=1;
	}
	//TODO if malloc return 0 -> reset avr + fatal error
	return malloc(n);
}

void operator delete(void * p)
{
  free(p);
}

//src: https://www.avrfreaks.net/forum/avr-c-micro-how?page=all
__extension__ typedef int __guard __attribute__((mode (__DI__)));
extern "C" int __cxa_guard_acquire(__guard *g){return !*(char *)(g);}
extern "C" void __cxa_guard_release (__guard *g){*(char *)g = 1;}
extern "C" void __cxa_guard_abort (__guard *){}
extern "C" void __cxa_pure_virtual(void){}
extern "C" void	atexit( void ) { }//µC will never "exit", so atexit can be dummy.


#endif /* ATMIGHTY_UTILITIES_C___FULLCPPSUPPORT_H_ */
