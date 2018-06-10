/*
 * This headerfile represents a common interface for all abstract 8bit Timers.
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_TIMER_ABSTRACTTIMER8BIT_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_TIMER_ABSTRACTTIMER8BIT_H_


#include "ATmighty/Ressources/Periphery/Abstract/Timer/Timer8bit.h"


///This class represents a common interface for all abstract 8bit Timers
class AbstractTimer8bit : public Timer8bit
{
	friend class AbstractHardwareManager;

	protected:
		/// Default Constructor - Can only be called by friends
		inline AbstractTimer8bit() {}

		/// Virtual Destructor
		virtual ~AbstractTimer8bit() {}

		/*!
		 * Frees this abstract hardware and the allocated physical hardware dependencies.
		 * This routine can only be called by friends.
		 */
		virtual void exit() = 0;

		/// Returns the current owner id
		virtual int8_t getOwner() = 0;

	public:
		/// Returns the corresponding number (upper-case) associated with the specific physical Timer used by this AbstractTimer8bit \returns ('0'-'9')
		virtual char getCharCode() = 0;
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_TIMER_ABSTRACTTIMER8BIT_H_ */
