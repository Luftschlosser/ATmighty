/*/
 * This headerfile defines a Trigger which can be configured to trigger either a Listener or a callback-function periodically.
 */

#ifndef ATMIGHTY_TOOLS_TIMING_PERIODICTRIGGER_PERIODICTRIGGER_H_
#define ATMIGHTY_TOOLS_TIMING_PERIODICTRIGGER_PERIODICTRIGGER_H_


#include "ATmighty/Interfaces/Listener.h"
#include <stdint.h>

//forward-declare dependencies
class Timer16bit;


/*!
 * This class represents a trigger source which can be configured to trigger either a Listener or a callback-function in equal time-intervals periodically.
 * The optional template parameter can be used to specify the specific type of Timer to be used for the periodic triggering. (default is any 16bit-Timer)
 * Because the Prescale-unit of the µC is running asynchronously, the first trigger might occur too early, but the interval between individual
 * triggering events will be precise.
 */
template<class Timer = Timer16bit> class PeriodicTrigger
{
	private:
		/// A reference to the abstract/virtual timer used by this PeriodicTrigger
		Timer* timer;

	public:
		/*!
		 * Constructor
		 * \param timer a reference to the abstract or virtual timer used for the periodic triggering. This Timer does need a minimum of 1 output-channel
		 * in order to function properly, as the first channel is used for CTC-Operation-mode.
		 */
		PeriodicTrigger(Timer* timer);

		///Destructor
		~PeriodicTrigger();

		/*!
		 * Sets the time-interval between each periodic triggering-event in terms of timer-steps.
		 * Setting the period while the triggering is already enabled might cause some inconsistent trigger-interval in between, as
		 * the timer-register responsible for that behavior is not double-buffered!
		 * \param timerSteps The number of single timer-steps between each triggering event. (Must be > 0)
		 * The duration of one timer-step depends on the specific timer used by this PeriodicTrigger,
		 * but will be equal to one cpu-cycle for non-virtual timers.
		 * \returns The precision which could be reached with the used timer in terms of timer-steps offset. A perfect period-configuration will
		 * return 0, anything else will mean that the actual period will be shifted by the returned number of timer-steps.
		 * (negative = delay too short / positive = delay too long)
		 * A return-value of -32768 (min) will mean that the desired period could not be approximated and the offset could be
		 * anything.
		 */
		int16_t setPeriod(uint32_t timerSteps);

		/*!
		 * Sets the time-interval between each periodic triggering-event in terms of seconds.
		 * Setting the period while the triggering is already enabled might cause some inconsistent trigger-interval in between, as
		 * the timer-register responsible for that behavior is not double-buffered!
		 * \param s The number of seconds between each triggering event. (Must be > 0)
		 * \returns The precision which could be reached with the used timer in terms of second offset. A perfect period-configuration will
		 * return 0, anything else will mean that the actual period will be shifted by the returned number of seconds.
		 * (negative = delay too short / positive = delay too long)
		 */
		int16_t setPeriodSeconds(uint16_t s);

		/*!
		 * Sets the time-interval between each periodic triggering-event in terms of milliseconds.
		 * Setting the period while the triggering is already enabled might cause some inconsistent trigger-interval in between, as
		 * the timer-register responsible for that behavior is not double-buffered!
		 * \param ms The number of milliseconds between each triggering event. (Must be > 0)
		 * \returns The precision which could be reached with the used timer in terms of millisecond offset. A perfect period-configuration will
		 * return 0, anything else will mean that the actual period will be shifted by the returned number of milliseconds.
		 * (negative = delay too short / positive = delay too long)
		 */
		int16_t setPeriodMilliseconds(uint16_t ms);

		/*!
		 * Sets the time-interval between each periodic triggering-event in terms of microseconds.
		 * Setting the period while the triggering is already enabled might cause some inconsistent trigger-interval in between, as
		 * the timer-register responsible for that behavior is not double-buffered!
		 * \param ms The number of microseconds between each triggering event. (Must be > 0)
		 * \returns The precision which could be reached with the used timer in terms of microsecond offset. A perfect period-configuration will
		 * return 0, anything else will mean that the actual period will be shifted by the returned number of microseconds.
		 * (negative = delay too short / positive = delay too long)
		 */
		int32_t setPeriodMicroseconds(uint32_t ms);

		/*!
		 * Sets the time-interval between each periodic triggering-event in terms of hertz. (triggers per second)
		 * Setting the period while the triggering is already enabled might cause some inconsistent trigger-interval in between, as
		 * the timer-register responsible for that behavior is not double-buffered!
		 * \param ms The number of triggers per second (Must be > 0)
		 * \returns The precision which could be reached with the used timer in terms of hertz offset. A perfect period-configuration will
		 * return 0, anything else will mean that the actual period will be shifted by the returned number of triggers per second.
		 * (negative = frequency too high / positive = frequency too low)
		 */
		int32_t setPeriodHertz(uint32_t hz);

		/*!
		 * Will calculate the maximal number of timerSteps which can be configured as one period with the specific used timer.
		 * This value might differ for abstract/virtual and 8bit/16bit-timers.
		 * \returns the maximal number of timer-steps which can be configured as one period between two triggering-events.
		 */
		uint32_t getMaxPeriod();

		///Will reset the used timer to start an new period. This may delay the next triggering event maximally by the duration of one period.
		inline void reset()
		{
			timer->setCounter(0);
		}

		/*!
		 * Enables triggering.
		 */
		inline void start()
		{
			timer->setCounter(0);
			timer->enableOutputCompareInterrupt(true, 'A');
		}

		/*!
		 * Disables triggering. This will NOT reset the internal timer, the timer will keep running.
		 */
		inline void stop()
		{
			timer->enableOutputCompareInterrupt(false, 'A');
		}

		/*!
		 * Sets a Listener-object reference as triggering target. The "trigger()"-function of the listener will be called on each
		 * triggering-event.
		 * \param listener A pointer to a listener-object which shall be triggered periodically by this PeriodicTrigger.
		 */
		inline void setTriggerAction(Listener* listener)
		{
			timer->setOutputCompareISR(listener, 'A');
		}

		/*!
		 * Sets a callback-function-pointer as triggering target. This function will be called on each triggering-event.
		 * \param callback A pointer to a function which shall be called periodically by this PeriodicTrigger.
		 */
		inline void setTriggerAction(void (*callback)())
		{
			timer->setOutputCompareISR(callback, 'A');
		}
};


#endif /* ATMIGHTY_TOOLS_TIMING_PERIODICTRIGGER_PERIODICTRIGGER_H_ */
