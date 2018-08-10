/*
 * This headerfile is µC-independent.
 */

#ifndef ATMIGHTY_TOOLS_COMMON_FINITESTATEMACHINE_FINITESTATEMACHINE_H_
#define ATMIGHTY_TOOLS_COMMON_FINITESTATEMACHINE_FINITESTATEMACHINE_H_


#include <stdint.h>
#include "ATmighty/Ressources/Interrupts/InterruptManager.h"


/*!
 * This class implements a finite Statemachine. The internal state is represented as an integer, but you may use a
 * well-defined enum (of type uint8_t) for a more comfortable use. The state can be changed by calling "changeState()".
 * When changing the state from inside one of the action-handlers (enter-, exit- or change-Actions), the change will not be
 * executed immediately but instead be buffered and executed after the current change is finished. When the state is changed
 * multiple times while a change is executed, only the last change will be executed.
 * Action-Handlers can only be set BEFORE "start()" was called. This will start the statemachine in a well-defined state and
 * will block any future changes of the handler-setup. This was implemented because changing the handlers themself while
 * running the statemachine could potentially lead to undefined behavior when using asynchronous design.
 */
class FiniteStatemachine
{
	private:
		///The number of the current state
		uint8_t state;

		///The number of the next state (used as buffer when state is changed while state is locked)
		uint8_t stateBuffer;

		///The total number of states
		uint8_t stateNumber;

		///Array of size \ref stateNumber*2, holds callback handler for enter/exit-actions for each state. (first enter-, then exit-actions in array).
		interruptHandler_t* statedActions;

		///Bitmap to store type of enter/exit-action handlers (1 = Listener, 0 = Callback, first enter-, then exit-actions in array).
		uint8_t* statedActionTypeMap;

		///callback-handler for state-change action
		interruptHandler_t changeAction;

		/*!
		 * Single-Bit values:
		 * Bit 0: stateLock (1 when state cannot be changed now, 0 when changeable)
		 * Bit 1: stateChangePending (1 when a changed state is pending in the stateBuffer, else 0)
		 * Bit 2: interruptHandlerType of changeAction-handler (1 = Listener, 0 = Callback)
		 * Bit 3: Actionhandler-Lockbit. Setting this bit 1 will prevent any further changes of the ActionHandlers.
		 */
		uint8_t stateFlags;

		/*!
		 * called internally when a state-change is executed.
		 * Blocks any incoming state-change requests from immediate execution (they will be buffered and executed iteratively instead).
		 */
		void stateChangeRoutine();

		///Locks the current state: this will prevent immediate state-changes (it will buffer them instead).
		inline void lockState() {stateFlags |= (1 << 0);}

		///Unlocks the current state: this will allow immediate state-changes.
		inline void unlockState() {stateFlags &= ~(1 << 0);}

		///Checks if the state is currently locked.
		inline bool isStateLocked() {return (stateFlags & (1 << 0));}

		///Sets the State-Change-Pending-Flag to signal that a state-change was buffered while immediate changes where blocked.
		inline void setPendingStatechange() {stateFlags |= (1 << 1);}

		///Clear the State-Change-Pending-Flag after handling a buffered state-change.
		inline void clearPendingStatechange() {stateFlags &= ~(1 << 1);}

		///Checks if a state-change is pending in the buffer currently.
		inline bool isStatechangePending() {return (stateFlags & (1 << 1));}

		///Sets the change-action-handler type bit to type "Listener"
		inline void setChangeactionHandlerTypeListener() {stateFlags |= (1 << 2);}

		///Sets the change-action-handler type bit to type "Callback"
		inline void setChangeactionHandlerTypeCallback() {stateFlags &= ~(1 << 2);}

		///Checks if the change-action-handler type is "Listener" (true) or "Callback" (false)
		inline bool isChangeactionHandlerTypeListener() {return stateFlags & (1 << 2);}

		///locks the current configuration of action-handlers by blocking any further changes.
		inline void lockActionHandlerConfiguration() {stateFlags |= (1 << 3);}

		///returns true when the ActionhandlerLockbit is set
		inline bool areActionhandlersLocked() {return stateFlags & (1 << 3);}

	public:
		/*!
		 * Contructor, allocates memory for the desired number of states
		 * \param stateNumber the total number of states.
		 */
		FiniteStatemachine(uint8_t stateNumber);

		///Destructor - Free's dynamically allocated memory
		~FiniteStatemachine();

		/*!
		 * Call this after the setup of all Action-Handlers is completed. It will enter the initial state.
		 * \param initialState the initial State to start the statemachine in: Value 0 to stateNumber-1.
		 */
		void start(uint8_t initialState);

		///Returns the current state of this Statemachine
		inline uint8_t getState() {return state;}

		/*!
		 * Changes the state of this Statemachine. When idle, the state will be changed immediately, when a statechange
		 * is currently executed, this nested change will be buffered and executed after the other state-change is finished.
		 * \param newState the value which the state shall be changed into. If equal to current state or not less than the
		 * total number of states, nothing will happen.
		 */
		void changeState(uint8_t newState);

		/*!
		 * Sets a Listener to be triggered on entering a specified state
		 * This will only work within the setup-phase of this statemachine, before "start()" is called!
		 * \param state The state which triggers the Listener when entered (range 0 to stateNumber-1, will to nothing when state is invalid)
		 * \param listener The Listener-object to be triggered when the specified state is entered.
		 */
		void setEnterAction(uint8_t state, Listener* listener);

		/*!
		 * Sets a Callback-Handler to be executed on entering a specified state
		 * This will only work within the setup-phase of this statemachine, before "start()" is called!
		 * \param state The state which calls the Handler when entered (range 0 to stateNumber-1, will to nothing when state is invalid)
		 * \param callback The functionpointer to be called when the specified state is entered.
		 */
		void setEnterAction(uint8_t state, void (*callback)());

		/*!
		 * Sets a Listener to be triggered on exiting a specified state
		 * This will only work within the setup-phase of this statemachine, before "start()" is called!
		 * \param state The state which triggers the Listener when exited (range 0 to stateNumber-1, will to nothing when state is invalid)
		 * \param listener The Listener-object to be triggered when the specified state is exited.
		 */
		void setExitAction(uint8_t state, Listener* listener);

		/*!
		 * Sets a Callback-Handler to be executed on exiting a specified state
		 * This will only work within the setup-phase of this statemachine, before "start()" is called!
		 * \param state The state which calls the Handler when exited (range 0 to stateNumber-1, will to nothing when state is invalid)
		 * \param callback The functionpointer to be called when the specified state is exited.
		 */
		void setExitAction(uint8_t state, void (*callback)());

		/*!
		 * Sets a Listener to be triggered on any state-change (Will be triggered after the enter- and exit-actions of the change were executed)
		 * This will only work within the setup-phase of this statemachine, before "start()" is called!
		 * \param listener The Listener-object which shall be triggered when a state-change occurs
		 */
		void setChangeAction(Listener* listener);

		/*!
		 * Sets a Callback-Handler to be executed on any state-change (Will be triggered after the enter- and exit-actions of the change were executed)
		 * This will only work within the setup-phase of this statemachine, before "start()" is called!
		 * \param callback The functionpointer which shall be called when a state-change occurs
		 */
		void setChangeAction(void (*callback)());
};


#endif /* ATMIGHTY_TOOLS_COMMON_FINITESTATEMACHINE_FINITESTATEMACHINE_H_ */
