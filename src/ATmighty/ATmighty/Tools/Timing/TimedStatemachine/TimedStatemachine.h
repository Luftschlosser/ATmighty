/*
 * This headerfile defines a finite Statemachine with n states represented as integers and provides Enter-, Exit- and Change-Events.
 * It also can be configured to change states automatically within specified intervals.
 */

#ifndef ATMIGHTY_TOOLS_TIMING_TIMEDSTATEMACHINE_TIMEDSTATEMACHINE_H_
#define ATMIGHTY_TOOLS_TIMING_TIMEDSTATEMACHINE_TIMEDSTATEMACHINE_H_


#include <stdint.h>
#include <stdlib.h>
#include <util/atomic.h>
#include "ATmighty/Interfaces/Listener.h"
#include "ATmighty/Tools/Timing/TimeoutTrigger/TimeoutTrigger.h"
#include "ATmighty/Tools/Timing/TimeUnitConverter/TimeUnitConverter.h"
#include "ATmighty/Tools/Common/FiniteStatemachine/FiniteStatemachine.h"


//forward declaration of used classes
class Timer16bit;


/*!
 * This class wrapps \see FiniteStatemachine and represents a finite statemachine itself, but with the ability to change its
 * state by itself using a \see TimeoutTrigger. Each automatic timed statechange can be configured with an individual delay.
 * The optional template parameter is used to define the Type of Timer which is used to drive the internally used \see TimeoutTrigger.
 * (default type is any 16-bit Timer)
 */
template<class Timer = Timer16bit> class TimedStatemachine final : private Listener
{
	private:
		///Structure definition for each timed statechange
		typedef struct stateChange
		{
			///The delay until the statechange in timer-cycles
			uint32_t delayCycles;

			///The new state after the statechange
			uint8_t targetState;

			///True when this statechange is set as timed statechange, false if not (manual)
			bool isSet;
		} stateChangeDefinition_t;

		///The statemachine which is used internally by this timed statemachine
		FiniteStatemachine statemachine;

		///The timeout trigger which is used to trigger the timed statechanges
		TimeoutTrigger<Timer> timeoutTrigger;

		///The time-unit-converter which is used to convert si-units of time into timer-cycles
		TimeUnitConverter<uint32_t> converter;

		///An Array which stores all timed statechanges, The index represents the current state to change from
		stateChangeDefinition_t *stateChanges;


		///Implements Listener::trigger - get called by the timeout-trigger to execute the next timed statechange
		void trigger();

		/*!
		 * Gets called when this statemachine comes to rest at any state (no pending statechanges left to execute).
		 * It will configure the timeout-trigger for the next timed statechange if any is set.
		 * \param state The current state this statemachine came to rest in.
		 */
		void reachedTerminalState(uint8_t state);

	public:
		/*!
		 * Constructor
		 * \param stateNumber the total number of states.
		 * \param timer the Timer used to drive this timedStatemachine (Type declared via template parameter) as the timed
		 * statechanges require the internal use of a TimeoutTrigger.
		 */
		TimedStatemachine(uint8_t stateNumber, Timer *timer);

		///Deconstructor (frees dynamically allocated memory)
		inline ~TimedStatemachine() { free(stateChanges); }

		/*!
		 * Use to calibrate the internally used \see TimeoutTrigger using an internal selftest.
		 * (But the trigger will be started AFTER the last statechange is executed completely, so there might be an delay/inaccuracy anyway)
		 * \returns the measured offset in cycles.
		 * (value will be constant in statically initialized environment, so it can be used directly to set the offset, rather than measuring every time)
		 */
		inline uint8_t calibrate() { return timeoutTrigger.calibrate(); }

		/*!
		 * Directly calibrates the internally used \see TimeoutTrigger by setting an offset value directly
		 * (Can be used in static environment after obtaining the measured value with the other "calibrate()"-method)
		 * \param offset The offset in cpu-cycles.
		 */
		inline void calibrate(uint8_t offset) { timeoutTrigger.calibrate(offset); }

		/*!
		 * Call this after the setup of all Action-Handlers and timed state-changes is completed. It will enter the initial state.
		 * \param initialState the initial State to start the statemachine in: Value 0 to stateNumber-1.
		 */
		inline void start(uint8_t initialState)
		{
			statemachine.start(initialState);
			reachedTerminalState(statemachine.getState());
		}

		///Returns the current state of this Statemachine
		inline uint8_t getState() {return statemachine.getState();}

		/*!
		 * Changes the state of this Statemachine.
		 * Will abort any running timed auto-statechanges.
		 * When idle, the state will be changed immediately, when a statechange is currently executed,
		 * this nested change will be buffered and executed after the other state-change is finished.
		 * \param newState the value which the state shall be changed into. If equal to current state or not less than the
		 * total number of states, the state will not be changed, but the timed auto-statechange will be reset if configured for this state.
		 */
		void changeState(uint8_t newState);

		/*!
		 * Sets up statechanges which will be automatically executed with a given delay.
		 * This is part of the setup-routine and can only be configured before "start()" is called.
		 * \param fromState the state from which this specific statechange starts
		 * \param toState the state which will come next
		 * \param delayCycles the number of timer-cycles which define the delay from entering the \ref fromState until the change is executed.
		 */
		void setTimedStatechange(uint8_t fromState, uint8_t toState, uint32_t delayCycles);

		/*!
		 * Sets a Listener to be triggered on entering a specified state
		 * This will only work within the setup-phase of this statemachine, before "start()" is called!
		 * \param state The state which triggers the Listener when entered (range 0 to stateNumber-1, will to nothing when state is invalid)
		 * \param listener The Listener-object to be triggered when the specified state is entered.
		 */
		inline void setEnterAction(uint8_t state, Listener* listener) { statemachine.setEnterAction(state, listener); }

		/*!
		 * Sets a Callback-Handler to be executed on entering a specified state
		 * This will only work within the setup-phase of this statemachine, before "start()" is called!
		 * \param state The state which calls the Handler when entered (range 0 to stateNumber-1, will to nothing when state is invalid)
		 * \param callback The functionpointer to be called when the specified state is entered.
		 */
		inline void setEnterAction(uint8_t state, void (*callback)()) { statemachine.setEnterAction(state, callback); }

		/*!
		 * Sets a Listener to be triggered on exiting a specified state
		 * This will only work within the setup-phase of this statemachine, before "start()" is called!
		 * \param state The state which triggers the Listener when exited (range 0 to stateNumber-1, will to nothing when state is invalid)
		 * \param listener The Listener-object to be triggered when the specified state is exited.
		 */
		inline void setExitAction(uint8_t state, Listener* listener) { statemachine.setExitAction(state, listener); }

		/*!
		 * Sets a Callback-Handler to be executed on exiting a specified state
		 * This will only work within the setup-phase of this statemachine, before "start()" is called!
		 * \param state The state which calls the Handler when exited (range 0 to stateNumber-1, will to nothing when state is invalid)
		 * \param callback The functionpointer to be called when the specified state is exited.
		 */
		inline void setExitAction(uint8_t state, void (*callback)()) { statemachine.setExitAction(state, callback); }

		/*!
		 * Sets a Listener to be triggered on any state-change (Will be triggered after the enter- and exit-actions of the change were executed)
		 * This will only work within the setup-phase of this statemachine, before "start()" is called!
		 * \param listener The Listener-object which shall be triggered when a state-change occurs
		 */
		inline void setChangeAction(Listener* listener) { statemachine.setChangeAction(listener); }

		/*!
		 * Sets a Callback-Handler to be executed on any state-change (Will be triggered after the enter- and exit-actions of the change were executed)
		 * This will only work within the setup-phase of this statemachine, before "start()" is called!
		 * \param callback The functionpointer which shall be called when a state-change occurs
		 */
		inline void setChangeAction(void (*callback)()) { statemachine.setChangeAction(callback); }
};


#endif /* ATMIGHTY_TOOLS_TIMING_TIMEDSTATEMACHINE_TIMEDSTATEMACHINE_H_ */
