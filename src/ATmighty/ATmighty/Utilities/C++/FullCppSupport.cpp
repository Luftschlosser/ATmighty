/*!
 * Implements functions and operators for full-c++-support
 */


#include "FullCppSupport.h"


///normal new
void * operator new(size_t n)
{
	if (n == 0)
	{
		n=1;
	}
	//TODO if malloc return 0 -> reset avr + fatal error
	return malloc(n);
}


///placement new.
void* operator new(size_t, void* __p) { return __p; }
void* operator new[](size_t, void* __p) { return __p; }


///normal delete
void operator delete(void * p)
{
  free(p);
}


///placement delete.
void  operator delete  (void*, void*) { }
void  operator delete[](void*, void*) { }


///other special-functions
extern "C" int __cxa_guard_acquire(__guard *g)
{
	return !*(char *)(g);
}
extern "C" void __cxa_guard_release (__guard *g)
{
	*(char *)g = 1;
}
extern "C" void __cxa_guard_abort (__guard *) { }
extern "C" void __cxa_pure_virtual(void) { }
extern "C" void	atexit( void ) { } //µC will never "exit", so atexit can be dummy.


