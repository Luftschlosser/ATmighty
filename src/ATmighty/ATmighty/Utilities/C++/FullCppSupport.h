/*!
 * This headerfile contains global functions to enable full c++-Support on AVR's
 */

#ifndef ATMIGHTY_UTILITIES_C___FULLCPPSUPPORT_H_
#define ATMIGHTY_UTILITIES_C___FULLCPPSUPPORT_H_


#include <stdlib.h>


///normal new
void * operator new(size_t n);

///placement new.
void* operator new(size_t, void* __p);
void* operator new[](size_t, void* __p);

///normal delete
void operator delete(void * p);

///placement delete.
void  operator delete  (void*, void*);
void  operator delete[](void*, void*);

///other special-functions
//src: https://www.avrfreaks.net/forum/avr-c-micro-how?page=all
__extension__ typedef int __guard __attribute__((mode (__DI__)));
extern "C" int __cxa_guard_acquire(__guard *g);
extern "C" void __cxa_guard_release (__guard *g);
extern "C" void __cxa_guard_abort (__guard *);
extern "C" void __cxa_pure_virtual(void);
extern "C" void	atexit( void );


#endif /* ATMIGHTY_UTILITIES_C___FULLCPPSUPPORT_H_ */
