/*!
 * This file implements the class \see PhysicalHardwareManager
 */

#include <avr/interrupt.h>
#include "Atmega2560PhysicalHardwareManager.h"
#include "ATmighty/Ressources/Periphery/Physical/Timer.h"


//Initialization of static free-hardware-items counter, initially the total number of hardware items.
volatile uint8_t PhysicalHardwareManager::allocatedHardwareItems = 0;

//Initialization of static hardware-item objects
Timer0 PhysicalHardwareManager::timer0;


//Get Owner-ID functions
template<class Hw> inline int8_t PhysicalHardwareManager::getOwnerId() {return 0;} 						//General case
template<> inline int8_t PhysicalHardwareManager::getOwnerId<Timer0>() {return timer0.getOwner();}		//Timer0


//Allocate hardware functions
template<class Hw> Hw* PhysicalHardwareManager::allocHardware(int8_t ownerID)		//General case
{
	//add log-message
	return nullptr;
}
template<> Timer0* PhysicalHardwareManager::allocHardware<Timer0>(int8_t ownerID)	//Timer0
{
	bool interruptFlag = (SREG & _BV(SREG_I)); //Save interrupt-flag

	if (ownerID == 0) {ownerID = -1;} //validate ownerID

	cli(); //clear interrupts (allocation must be atomic)
	if (timer0.isFree())
	{
		timer0.allocate(ownerID);
		if (interruptFlag) {sei();} //restore interrupt-flag
		allocatedHardwareItems++;
		instanceUsageRelation++;
		//add log-message
		return &timer0;
	}
	else
	{
		if (interruptFlag) {sei();} //restore interrupt-flag
		//add log-message
		return nullptr;
	}
}


//Free hardware function
void PhysicalHardwareManager::free(PhysicalHardwareBase **hardware)
{
	if (hardware != nullptr && (*hardware) != nullptr)
	{
		(*hardware)->free();
		allocatedHardwareItems--;
		instanceUsageRelation--;
		(*hardware) = nullptr;
	}
	return;
}
