/*
 * This headerfile defines several string-literals and helper functions which are used by ATmighty to set
 * senseful OwnerID's for hardware-allocations and retrieve a senseful description for each ID for logging.
 */

#ifndef ATMIGHTY_UTILITIES_LUTS_HARDWAREOWNERID_H_
#define ATMIGHTY_UTILITIES_LUTS_HARDWAREOWNERID_H_


#include <avr/pgmspace.h>


namespace OwnerID
{
	//ATmighty OwnerID's have to be negative numbers, as the positive range is reserved for the user.

	///This enum defines different usages/OwnerID's for ATmighty's hardware-allocations
	enum OwnerID : int8_t
	{
		Unused = 0,					//Hw is actually not in use at all (free)
		Default = -1,				//Fallback OwnerID (gets used when HW is allocated by OwnerID 0)
		DirectAbstraction = -2,		//physical HW is used as an abstract HW of the same Type (e.g. PortA as IoPort->AbstractIoPort->AbstractPortA)
		IndirectAbstraction = -3,	//physical HW is partially used by an abstract HW of a dependent type Type (e.g. PortA by IoPin->AbstractIoPin->AbstractPinA0)
		AbstractionDependency = -4,	//HW is used by another abstract HW (e.g. AbstractIoPin as PWM-output by AbstractTimer8b)
		MsgLogWriter = -5,			//HW is used by MessageLogWriter-instance
	};


	/*!
	* retrieves a string representation of an OwnerID for logging.
	* \param id the owner-id to get as string
	* \returns a PGM_P type pointer which points to the address in PROGMEM where the string is stored, or nullptr if the given id is not defined as string.
	*/
	PGM_P GetOwnerIdDescription(int8_t id);
}


#endif /* ATMIGHTY_UTILITIES_LUTS_HARDWAREOWNERID_H_ */
