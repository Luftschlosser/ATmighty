/*!
 * Implements \see TimeoutTrigger-functions which are timer-independent
 */


#include "TimeoutTrigger.h"
#include "ATmighty/Tools/Timing/TimeUnitConverter/TimeUnitConverter.h"
#include "ATmighty/Ressources/Periphery/Abstract/Timer.h"
#include "ATmighty/Ressources/Periphery/Virtual/Timer/VirtualTimer8bit.h"
#include "ATmighty/Utilities/Logs/MessageLog.h"
#include "ATmighty/Utilities/LUTs/MessageLogPhrases.h"


template <class Timer> TimeoutTrigger<Timer>::~TimeoutTrigger()
{
	timer->stop();
	timer->enableOutputCompareInterrupt(false, 'A');
	timer->setOutputCompareISR((Listener*)nullptr, 'A');
}

template <class Timer> void TimeoutTrigger<Timer>::trigger()
{
	timer->enableOutputCompareInterrupt(false, 'A');
	InterruptManager::enableNesting();
	clearRunning();
	if (triggerAction.callback != nullptr)
	{
		if (isTriggerActionListener())
		{
			triggerAction.listener->trigger();
		}
		else
		{
			triggerAction.callback();
		}
	}
}

template <class Timer> void TimeoutTrigger<Timer>::start()
{
	setRunning();
	timer->setCounter(0);
	timer->enableOutputCompareInterrupt(true, 'A');
}

template <class Timer> void TimeoutTrigger<Timer>::cancel()
{
	if (isRunning())
	{
		timer->enableOutputCompareInterrupt(false, 'A');
		clearRunning();
	}
}

template <class Timer> void TimeoutTrigger<Timer>::setTriggerAction(Listener* listener)
{
	if(!isRunning())
	{
		triggerAction.listener = listener;
		setTriggeractionTypeListener();
	}
}

template <class Timer> void TimeoutTrigger<Timer>::setTriggerAction(void (*callback)())
{
	if(!isRunning())
	{
		triggerAction.callback = callback;
		setTriggeractionTypeCallback();
	}
}

template <class Timer> int16_t TimeoutTrigger<Timer>::setTimespanSeconds(uint16_t s)
{
	if(!isRunning())
	{
		TimeUnitConverter<uint32_t> converter(timer->getBaseFrequency());
		uint32_t timerSteps;

		//This function doesn't work for asynchronous timer-mode!
		if (converter.getBaseFrequency() == 0)
		{
			return -32768;
		}

		//Calculate needed number of timerSteps
		timerSteps = converter.secondsToCycles(s);

		if (timerSteps > getMaxTimespan())
		{
			timerSteps = getMaxTimespan();
			#if ATMIGHTY_MESSAGELOG_ENABLE
			MessageLog<>::DefaultInstance().log<LogLevel::Error>(true,
					MessageLogPhrases::Text_TimeoutTriggerUnapproximatable,
					converter.cyclesToSeconds(timerSteps),
					MessageLogPhrases::Text_UnitSeconds);
			#endif
		}

		//setting period
		timerSteps -= setTimespan(timerSteps);

		//calculate actual period time in seconds
		return (int16_t)((int16_t)converter.cyclesToSeconds(timerSteps) - s);
	}
	else
	{
		return 0;
	}
}

template <class Timer> int16_t TimeoutTrigger<Timer>::setTimespanMilliseconds(uint16_t ms)
{
	if(!isRunning())
	{
		TimeUnitConverter<uint32_t> converter(timer->getBaseFrequency());
		uint32_t timerSteps;

		//This function doesn't work for asynchronous timer-mode!
		if (converter.getBaseFrequency() == 0)
		{
			return -32768;
		}

		//Calculate needed number of timerSteps
		timerSteps = converter.millisecondsToCycles(ms);

		if (timerSteps > getMaxTimespan())
		{
			timerSteps = getMaxTimespan();
			#if ATMIGHTY_MESSAGELOG_ENABLE
			MessageLog<>::DefaultInstance().log<LogLevel::Error>(true,
					MessageLogPhrases::Text_TimeoutTriggerUnapproximatable,
					converter.cyclesToMilliseconds(timerSteps),
					MessageLogPhrases::Text_UnitMilliseconds);
			#endif
		}

		//setting period
		timerSteps -= setTimespan(timerSteps);

		//calculate actual period time in milliseconds
		return (int16_t)((int16_t)converter.cyclesToMilliseconds(timerSteps) - ms);
	}
	else
	{
		return 0;
	}
}

template <class Timer> int32_t TimeoutTrigger<Timer>::setTimespanMicroseconds(uint32_t ms)
{
	if(!isRunning())
	{
		TimeUnitConverter<uint32_t> converter(timer->getBaseFrequency());
		uint32_t timerSteps;

		//This function doesn't work for asynchronous timer-mode!
		if (converter.getBaseFrequency() == 0)
		{
			return -2147483648;
		}

		//Calculate needed number of timerSteps
		timerSteps = converter.microsecondsToCycles(ms);

		if (timerSteps > getMaxTimespan())
		{
			timerSteps = getMaxTimespan();
			#if ATMIGHTY_MESSAGELOG_ENABLE
			uint64_t maxMs = converter.cyclesToMicroseconds(timerSteps);
			if (maxMs > 0xFFFFFFFF)
			{
				MessageLog<>::DefaultInstance().log<LogLevel::Error>(true,
					MessageLogPhrases::Text_TimeoutTriggerUnapproximatable,
					0xFFFFFFFF, '+',
					MessageLogPhrases::Text_UnitMicroseconds);
			}
			else
			{
				MessageLog<>::DefaultInstance().log<LogLevel::Error>(true,
					MessageLogPhrases::Text_TimeoutTriggerUnapproximatable,
					(uint32_t)maxMs,
					MessageLogPhrases::Text_UnitMicroseconds);
			}
			#endif
		}

		//setting period
		timerSteps -= setTimespan(timerSteps);

		//calculate actual period time in milliseconds
		return (int32_t)((int32_t)converter.cyclesToMicroseconds(timerSteps) - ms);
	}
	else
	{
		return 0;
	}
}


//Explizit template instantiations for this µC
template class TimeoutTrigger<Timer16bit>;
template class TimeoutTrigger<AbstractTimer16bit>;
//template class TimeoutTrigger<VirtualTimer16bit>;
template class TimeoutTrigger<Timer8bit>;
template class TimeoutTrigger<AbstractTimer8bit>;
template class TimeoutTrigger<AbstractTimer8bitAsync>;
template class TimeoutTrigger<VirtualTimer8bit>;
#if defined (__AVR_ATmega2560__)
template class TimeoutTrigger<AbstractTimer0>;
template class TimeoutTrigger<AbstractTimer1>;
template class TimeoutTrigger<AbstractTimer2>;
template class TimeoutTrigger<AbstractTimer3>;
template class TimeoutTrigger<AbstractTimer4>;
template class TimeoutTrigger<AbstractTimer5>;
#else
#  warning "No specialized TimeoutTriggers defined for this µC."
#endif
