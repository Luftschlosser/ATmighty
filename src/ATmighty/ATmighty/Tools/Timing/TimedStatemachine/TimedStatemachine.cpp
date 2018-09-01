/*
 * Implements \see TimedStatemachine
 */


#include "TimedStatemachine.h"
#include "ATmighty/Ressources/Periphery/Abstract/Timer.h"
#include "ATmighty/Ressources/Periphery/Virtual/Timer/VirtualTimer8bit.h"


template<class Timer> TimedStatemachine<Timer>::TimedStatemachine(uint8_t stateNumber, Timer *timer) :
	statemachine(stateNumber),
	timeoutTrigger(timer),
	converter(timer->getBaseFrequency()),
	stateChanges(nullptr)
{
	stateChanges = (stateChangeDefinition_t*)calloc(stateNumber, sizeof(stateChangeDefinition_t));
	if (stateChanges != nullptr)
	{
		timeoutTrigger.setTriggerAction((Listener*)this);
	}
	else
	{
		//Todo:error
	}
}

template<class Timer> void TimedStatemachine<Timer>::trigger()
{
	uint8_t newState = stateChanges[statemachine.getState()].targetState;
	statemachine.changeState(newState);
	reachedTerminalState(statemachine.getState());
}

template<class Timer> void TimedStatemachine<Timer>::reachedTerminalState(uint8_t state)
{
	if (stateChanges[state].isSet)
	{
		timeoutTrigger.setTimespan(stateChanges[state].delayCycles);
		timeoutTrigger.start();
	}
}

template<class Timer> void TimedStatemachine<Timer>::changeState(uint8_t newState)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		timeoutTrigger.cancel();
	}
	statemachine.changeState(newState);
	reachedTerminalState(statemachine.getState());
}

template<class Timer> void TimedStatemachine<Timer>::setTimedStatechange(uint8_t fromState, uint8_t toState, uint32_t delayCycles)
{
	if (!statemachine.areActionhandlersLocked())
	{
		stateChanges[fromState].delayCycles = delayCycles;
		stateChanges[fromState].targetState = toState;
		stateChanges[fromState].isSet = true;
	}
}


//Explizit template instantiations for this µC
template class TimedStatemachine<Timer16bit>;
template class TimedStatemachine<AbstractTimer16bit>;
//template class TimedStatemachine<VirtualTimer16bit>;
template class TimedStatemachine<Timer8bit>;
template class TimedStatemachine<AbstractTimer8bit>;
template class TimedStatemachine<AbstractTimer8bitAsync>;
template class TimedStatemachine<VirtualTimer8bit>;
#if defined (__AVR_ATmega2560__)
template class TimedStatemachine<AbstractTimer0>;
template class TimedStatemachine<AbstractTimer1>;
template class TimedStatemachine<AbstractTimer2>;
template class TimedStatemachine<AbstractTimer3>;
template class TimedStatemachine<AbstractTimer4>;
template class TimedStatemachine<AbstractTimer5>;
#else
#  warning "No specialized TimedStatemachines defined for this µC."
#endif
