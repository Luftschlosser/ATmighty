/*!
 * Implements \see FiniteStatemachine
 */


#include "FiniteStatemachine.h"
#include <stdlib.h>
#include "ATmighty/Interfaces/Listener.h"
#include "ATmighty/Utilities/Logs/MessageLog.h"
#include <util/atomic.h>


FiniteStatemachine::FiniteStatemachine(uint8_t stateNumber)
	: state(0),
	  stateBuffer(0),
	  stateNumber(stateNumber),
	  statedActions(nullptr),
	  statedActionTypeMap(nullptr),
	  stateFlags(0)
{
	statedActions = (interruptHandler_t*)calloc(stateNumber * 2, sizeof(interruptHandler_t));
	uint8_t neededBytes = (stateNumber / 4) + ((stateNumber % 4 == 0) ? 0 : 1);
	statedActionTypeMap = (uint8_t*)calloc(neededBytes, sizeof(uint8_t));

	if ((statedActions == nullptr) || (statedActionTypeMap == nullptr))
	{
		//todo implement properly
		MessageLog<>::DefaultInstance().log<LogLevel::Fatal>(false, "malloc failed!");
	}
}

FiniteStatemachine::~FiniteStatemachine()
{
	free(statedActionTypeMap);
	free(statedActions);
}

void FiniteStatemachine::stateChangeRoutine()
{
	bool iterate;

	//disallow direct state-changes while changing state and executing callback-handlers
	//intermediate state-changes will be buffered and executed iterative...
	lockState();

	//change state as requested, until no nested state-changes are pending anymore.
	//(iterative approach to prevent stack-overflow from recursion of nested state-changes)
	do
	{
		uint8_t newState = stateBuffer;
		clearPendingStatechange();

		//exit-action
		uint8_t bitNr = state + stateNumber;
		if (statedActions[bitNr].callback != nullptr)
		{

			if (statedActionTypeMap[bitNr / 8] & (1 << (bitNr % 8)))
			{
				statedActions[bitNr].listener->trigger();
			}
			else
			{
				statedActions[bitNr].callback();
			}
		}

		state = newState;

		//enter-action
		if (statedActions[newState].callback != nullptr)
		{
			if (statedActionTypeMap[newState / 8] & (1 << (newState % 8)))
			{
				statedActions[newState].listener->trigger();
			}
			else
			{
				statedActions[newState].callback();
			}
		}

		//change-action
		if (changeAction.callback != nullptr)
		{
			if(isChangeactionHandlerTypeListener())
			{
				changeAction.listener->trigger();
			}
			else
			{
				changeAction.callback();
			}
		}

		//Iterate? (must be atomic together with unlockState(), as this could lead to missed state-changes otherwise).
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			iterate = isStatechangePending();
			if(!iterate)
			{
				//state-change finished, including all nested state-changes. Allow direct state-changes again.
				unlockState();
			}
		}

	} while (iterate);
}

void FiniteStatemachine::start(uint8_t initialState)
{
	lockState();

	state = (initialState < stateNumber) ? initialState : 0;

	lockActionHandlerConfiguration();

	//enter-action
	if (statedActions[state].callback != nullptr)
	{
		if (statedActionTypeMap[state / 8] & (1 << (state % 8)))
		{
			statedActions[state].listener->trigger();
		}
		else
		{
			statedActions[state].callback();
		}
	}

	unlockState();
	if (isStatechangePending())
	{
		stateChangeRoutine();
	}
}

void FiniteStatemachine::changeState(uint8_t newState)
{
	if ((newState < stateNumber) && (newState != state))
	{
		setPendingStatechange();
		stateBuffer = newState;

		if (!isStateLocked()) //state unlocked?
		{
			stateChangeRoutine();
		}
	}
}

void FiniteStatemachine::setEnterAction(uint8_t state, Listener* listener)
{
	if(!areActionhandlersLocked())
	{
		statedActionTypeMap[state / 8] |= (1 << (state % 8));
		statedActions[state].listener = listener;
	}
}

void FiniteStatemachine::setEnterAction(uint8_t state, void (*callback)())
{
	if(!areActionhandlersLocked())
	{
		statedActionTypeMap[state / 8] &= ~(1 << (state % 8));
		statedActions[state].callback = callback;
	}
}

void FiniteStatemachine::setExitAction(uint8_t state, Listener* listener)
{
	if(!areActionhandlersLocked())
	{
		uint8_t index = state + stateNumber;
		statedActionTypeMap[index / 8] |= (1 << (index % 8));
		statedActions[index].listener = listener;
	}
}

void FiniteStatemachine::setExitAction(uint8_t state, void (*callback)())
{
	if(!areActionhandlersLocked())
	{
		uint8_t index = state + stateNumber;
		statedActionTypeMap[index / 8] &= ~(1 << (index % 8));
		statedActions[index].callback = callback;
	}
}

void FiniteStatemachine::setChangeAction(Listener* listener)
{
	if(!areActionhandlersLocked())
	{
		setChangeactionHandlerTypeListener();
		changeAction.listener = listener;
	}
}

void FiniteStatemachine::setChangeAction(void (*callback)())
{
	if(!areActionhandlersLocked())
	{
		setChangeactionHandlerTypeCallback();
		changeAction.callback = callback;
	}
}

