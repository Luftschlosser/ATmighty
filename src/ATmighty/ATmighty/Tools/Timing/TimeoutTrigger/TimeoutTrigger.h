/*/
 * This headerfile defines a Trigger which can be configured to trigger either a Listener or a callback-function once after a specific time.
 */

#ifndef ATMIGHTY_TOOLS_TIMING_TIMEOUTTRIGGER_TIMEOUTTRIGGER_H_
#define ATMIGHTY_TOOLS_TIMING_TIMEOUTTRIGGER_TIMEOUTTRIGGER_H_


#include "ATmighty/Interfaces/Listener.h"
#include "ATmighty/Ressources/Interrupts/InterruptManager.h"
#include <stdint.h>

//forward-declare dependencies
class Timer16bit;


/*!
 * This class represents a trigger source which can be configured to trigger either a Listener or a callback-function once after a specified time.
 * The optional template parameter can be used to specify the specific type of Timer to be used for the triggering. (default is any 16bit-Timer)
 */
template<class Timer = Timer16bit> class TimeoutTrigger final : private Listener
{
	private:
		///A reference to the abstract/virtual timer used by this PeriodicTrigger
		Timer* timer;

		///The handler which shall be executed upon timeout
		interruptHandler_t triggerAction;

		///constant Offset to all timespan-settings
		int16_t calibrationOffset;

		/*!
		 * Bitmask for multiple values:
		 * Bit0: Running ->	1 when this TimeoutTrigger is currently active and ready to trigger on timeout,
		 * 					0 if the timer is not running or the timeout event already occurred.
		 * 					TimeoutTrigger can only be configured when not running.
		 * Bit1: triggeractionHandlerType -> 1 = Listener, 0 = Callback.
		 */
		uint8_t statusFlags;


		///Sets this TimeoutTrigger's "Running"-Flag to signal that it is active
		inline void setRunning() { statusFlags |= (1 << 0); }

		///Clears this TimeoutTrigger's "Running"-Flag to signal that it is inactive and can be configured (before start / after timeout event occurred)
		inline void clearRunning() { statusFlags &= ~(1 << 0); }

		///Sets the triggeraction-handler-type Flag to type "Listener"
		inline void setTriggeractionTypeListener() { statusFlags |= (1 << 1); }

		///Sets the triggeraction-handler-type Flag to type "Callback"
		inline void setTriggeractionTypeCallback() { statusFlags &= ~(1 << 1); }

		///Checks if the triggeraction-handler-type is type "Listener"
		inline bool isTriggerActionListener() { return statusFlags & (1 << 1); }

		///Listener::trigger() -> gets triggered by timer, deactivates timer and executes trigger-action.
		virtual void trigger();

	public:
		/*!
		 * Constructor
		 * \param timer a reference to the abstract or virtual timer used for the triggering. This Timer does need a minimum of 1 output-channel
		 * in order to function properly, as the first channel is used for CTC-Operation-mode.
		 */
		TimeoutTrigger(Timer* timer);

		///Destructor
		~TimeoutTrigger();

		/*!
		 * Directly sets an offset value (Can be used in static environment after obtaining the measured value with the other "calibrate()"-method)
		 * \param offset The offset in cpu-cycles.
		 */
		inline void calibrate(int16_t offset) { calibrationOffset = offset; }

		/*!
		 * Sets the timespan between starting this TimeoutTrigger and the triggering-event in terms of timer-steps.
		 * It is only possible to configure this timespan while the TimeoutTrigger is not running!
		 * \param timerSteps The number of single timer-steps until the triggering event. (Must be > 0)
		 * The duration of one timer-step depends on the specific timer used by this PeriodicTrigger,
		 * but will be equal to one cpu-cycle for non-virtual timers.
		 * \returns The precision which could be reached with the used timer in terms of timer-steps offset. A perfect timespan-configuration will
		 * return 0, anything else will mean that the actual timeout event will be shifted by the returned number of timer-steps.
		 * (negative = delay too short / positive = delay too long)
		 * A return-value of -32768 (min) will mean that the desired period could not be approximated and the offset could be
		 * anything.
		 */
		int16_t setTimespan(uint32_t timerSteps);

		/*!
		 * Sets the timespan between starting this TimeoutTrigger and the triggering-event in terms of seconds.
		 * It is only possible to configure this timespan while the TimeoutTrigger is not running!
		 * \param ms The number of seconds until the triggering event. (Must be > 0)
		 * \returns The precision which could be reached with the used timer in terms of second offset. A perfect timespan-configuration will
		 * return 0, anything else will mean that the timeout event will be shifted by the returned number of seconds.
		 * (negative = delay too short / positive = delay too long)
		 */
		int16_t setTimespanSeconds(uint16_t s);

		/*!
		 * Sets the timespan between starting this TimeoutTrigger and the triggering-event in terms of milliseconds.
		 * It is only possible to configure this timespan while the TimeoutTrigger is not running!
		 * \param ms The number of milliseconds until the triggering event. (Must be > 0)
		 * \returns The precision which could be reached with the used timer in terms of millisecond offset. A perfect timespan-configuration will
		 * return 0, anything else will mean that the actual timeout event will be shifted by the returned number of milliseconds.
		 * (negative = delay too short / positive = delay too long)
		 */
		int16_t setTimespanMilliseconds(uint16_t ms);

		/*!
		 * Sets the timespan between starting this TimeoutTrigger and the triggering-event in terms of microseconds.
		 * It is only possible to configure this timespan while the TimeoutTrigger is not running!
		 * \param ms The number of microseconds until the triggering event. (Must be > 0)
		 * \returns The precision which could be reached with the used timer in terms of microsecond offset. A perfect timespan-configuration will
		 * return 0, anything else will mean that the actual timeout event will be shifted by the returned number of microseconds.
		 * (negative = delay too short / positive = delay too long)
		 */
		int32_t setTimespanMicroseconds(uint32_t ms);

		/*!
		 * Will calculate the maximal number of timerSteps which can be configured as one timepan with the specific used timer.
		 * This value might differ for abstract/virtual and 8bit/16bit-timers.
		 * \returns the maximal number of timer-steps which can be configured as one period between two triggering-events.
		 */
		uint32_t getMaxTimespan();

		/*!
		 * Enables and resets triggering. This will start the TimeoutTrigger with the preconfigured timespan and timeout-action.
		 * If the TimeoutTrigger is currently running, this will reset the Trigger by waiting again the entire configured timespan before triggering.
		 * If the TimeoutTrigger is currently not running, it will start running with the preconfigured timespan and timeout-action.
		 */
		void start();

		///Disables triggering when this TimeoutTrigger is currently running.
		void cancel();

		///Ckecks if thos TimeoutTrigger is currently running (active, true) or not (configurable, false).
		inline bool isRunning()
		{
			return statusFlags & (1 << 0);
		}

		/*!
		 * Sets a Listener-object reference as triggering target. The "trigger()"-function of the listener will be called on each
		 * triggering-event. Setting this is only possible when the TimeoutTrigger is currently not running!
		 * \param listener A pointer to a listener-object which shall be triggered at the timeout event of this TimeoutTrigger.
		 */
		void setTriggerAction(Listener* listener);

		/*!
		 * Sets a callback-function-pointer as triggering target. This function will be called on each triggering-event.
		 * Setting this is only possible when the TimeoutTrigger is currently not running!
		 * \param callback A pointer to a function which shall be called at the timeout event of this TimeoutTrigger.
		 */
		void setTriggerAction(void (*callback)());
};


#endif /* ATMIGHTY_TOOLS_TIMING_TIMEOUTTRIGGER_TIMEOUTTRIGGER_H_ */
