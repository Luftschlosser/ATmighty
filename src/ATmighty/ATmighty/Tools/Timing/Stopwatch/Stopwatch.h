/*/
 * This headerfile defines a StopWatch, which can be used to measure the number of cpu-cycles which pass between to function-calls.
 */

#ifndef ATMIGHTY_TOOLS_TIMING_STOPWATCH_H_
#define ATMIGHTY_TOOLS_TIMING_STOPWATCH_H_


#include "ATmighty/Interfaces/Listener.h"
#include <stdint.h>


//forward-declare dependencies
class AbstractTimer16bit;


/*!
 * This class represents a stop-watch which can be used to measure cycle-precise delays between the call to start() and the call to stop().
 * The template-parameter "Timer" defines the type of timer to be used for measurement. This must be an abstract Timer, as virtual ones will
 * not be fast enough for cycle-precise measurements. It is recommended to provide a specific timer-type if it is known at compiletime, as this
 * will reduce the time needed to call the timer-methods significantly because no vtable-entries will need to be dereferenced.
 * Possible values are:
 * AbstractTimer16bit [default], AbtractTimer8bit, and all specific AbstractTimerN's of this µC.
 * For the Stopwatch to be able to measure consistently precise values, it is recommended to not use any interrupts which might occur during
 * measurement. But Interrupts need to stay enabled, as the Stopwatch itself does need timer-interrupts to work correctly.
 */
template<class Timer = AbstractTimer16bit> class Stopwatch
{
	private:
		/*!
		 * This nested class is used to count the number of overflows that appear while the StopWatch is counting
		 */
		class Overflower : public Listener
		{
			private:
				/// the number of counted overflows
				volatile uint16_t overflows;

			public:
				/// default Constructor
				inline Overflower() : overflows(0) {}

				/// resets the overflow-counter to 0
				inline void reset() {overflows = 0;}

				/// returns the number of counted overflows
				inline uint16_t getOverflowCount() {return overflows;}

				/// increments overflow-counter, gets triggered by Timer-overflow-interrupt
				void trigger();
		};

		/// A reference to the abstract timer used by this StopWatch
		Timer* timer;

		/// The Overflow-counter instance
		Overflower overflowCounter;

		/// The offset which calibrates the measurement to be cycle-precise
		uint16_t offset;

		/*!
		 * This function is used to calibrate the Stopwatch for any given abstract timer.
		 * An indirect, static implementation is necessary as the start() and stop() methods must be called via a reference,
		 * from outside the instance to get accurate results which behave in the same manner as for any user.
		 * \returns the offset of an measurement with the given timer in cpu-cycles.
		 */
		__attribute__((noinline)) static uint32_t Calibrate(Stopwatch* reference);

	public:
		/*!
		 * Constructor, will initialize and calibrate this StopWatch for cycle-precise measurement
		 * \param timer An instance of the specified timer-type to be used by this timer.
		 * Needs to be an abstract timer, virtual ones will not work for cycle-precise measurement.
		 */
		Stopwatch(Timer* timer);

		/// resets and starts this StopWatch
		void start();

		/*!
		 * Measures the current time of this StopWatch. (Doesn't really stop it)
		 * It is possible to call this function multiple times without calling start() in between,
		 * if you want to take multiple measurements with the same starting-time.
		 * \returns the precise number of cycles which passed between the call to start() and the call to stop()
		 */
		uint32_t stop();
};


#endif /* ATMIGHTY_TOOLS_TIMING_STOPWATCH_H_ */
