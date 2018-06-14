/*
 * This headerfile represents a common interface for all abstract 16bit Timers.
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_TIMER_ABSTRACTTIMER16BIT_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_TIMER_ABSTRACTTIMER16BIT_H_


#include "ATmighty/Ressources/Periphery/Abstract/Timer/Timer16bit.h"


///This class represents a common interface for all abstract 16bit Timers
class AbstractTimer16bit : public Timer16bit
{
	friend class AbstractHardwareManager;

	protected:
		/// Default Constructor - Can only be called by friends
		inline AbstractTimer16bit() {}

		/// Virtual Destructor
		virtual ~AbstractTimer16bit() {}

		/*!
		 * Frees this abstract hardware and the allocated physical hardware dependencies.
		 * This routine can only be called by friends.
		 */
		virtual void exit() = 0;

		/// Returns the current owner id
		virtual int8_t getOwner() = 0;

	public:
		/// Returns the corresponding number (upper-case) associated with the specific physical Timer used by this AbstractTimer16bit \returns ('0'-'9')
		virtual char getCharCode() = 0;
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_TIMER_ABSTRACTTIMER16BIT_H_ */
