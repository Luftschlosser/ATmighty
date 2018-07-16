/*!
 * Implements \see PeriodicTrigger-functions which are timer-independent
 */


#include <ATmighty/Tools/Timing/PeriodicTrigger/PeriodicTrigger.h>
#include "ATmighty/Ressources/Periphery/Abstract/Timer.h"
#include "ATmighty/Utilities/Logs/MessageLog.h"
#include "ATmighty/Utilities/LUTs/MessageLogPhrases.h"
#include "Config/MessageLogConfig.h"


template <class Timer> PeriodicTrigger<Timer>::~PeriodicTrigger()
{
	stop();
	setTriggerAction((Listener*)nullptr);
}

template <class Timer> int16_t PeriodicTrigger<Timer>::setPeriodSeconds(uint16_t s)
{
	uint32_t baseFrequency = timer->getBaseFrequency();
	uint32_t timerSteps;

	//This function doesn't work for asynchronous timer-mode!
	if (baseFrequency == 0)
	{
		return -32768;
	}

	//Calculate needed number of timerSteps
	timerSteps = (baseFrequency * s);

	if (timerSteps > getMaxPeriod())
	{
		timerSteps = getMaxPeriod();
		#if ATMIGHTY_MESSAGELOG_ENABLE
		uint32_t maxMs = (timerSteps / baseFrequency);
		MessageLog<>::DefaultInstance().log<LogLevel::Error>(true,
				MessageLogPhrases::Text_PeriodicTriggerUnapproximatable,
				maxMs,
				MessageLogPhrases::Text_UnitSeconds);
		#endif
	}

	//setting period
	timerSteps -= setPeriod(timerSteps);

	//calculate actual period time in milliseconds
	return (int16_t)((int16_t)(timerSteps / baseFrequency) - s);
}

template <class Timer> int16_t PeriodicTrigger<Timer>::setPeriodMilliseconds(uint16_t ms)
{
	uint32_t baseFrequency = timer->getBaseFrequency();
	uint32_t timerSteps;

	//This function doesn't work for asynchronous timer-mode!
	if (baseFrequency == 0)
	{
		return -32768;
	}

	//Calculate needed number of timerSteps
	timerSteps = (((uint64_t)baseFrequency * ms) / 1000); //need temporary 64-bit for high precision

	if (timerSteps > getMaxPeriod())
	{
		timerSteps = getMaxPeriod();
		#if ATMIGHTY_MESSAGELOG_ENABLE
		uint32_t maxMs = (((uint64_t)timerSteps * 1000) / baseFrequency); //need temporary 64-bit for high precision
		MessageLog<>::DefaultInstance().log<LogLevel::Error>(true,
				MessageLogPhrases::Text_PeriodicTriggerUnapproximatable,
				maxMs,
				MessageLogPhrases::Text_UnitMilliseconds);
		#endif
	}

	//setting period
	timerSteps -= setPeriod(timerSteps);

	//calculate actual period time in milliseconds
	return (int16_t)((int16_t)(((uint64_t)timerSteps * 1000) / baseFrequency) - ms); //need temporary 64-bit for high precision
}

template <class Timer> int32_t PeriodicTrigger<Timer>::setPeriodMicroseconds(uint32_t ms)
{
	uint32_t baseFrequency = timer->getBaseFrequency();
	uint32_t timerSteps;

	//This function doesn't work for asynchronous timer-mode!
	if (baseFrequency == 0)
	{
		return -2147483648;
	}

	//Calculate needed number of timerSteps
	timerSteps = (((uint64_t)baseFrequency * ms) / 1000000); //need temporary 64-bit for high precision

	if (timerSteps > getMaxPeriod())
	{
		timerSteps = getMaxPeriod();
		#if ATMIGHTY_MESSAGELOG_ENABLE
		uint64_t maxMs = (((uint64_t)timerSteps * 1000000) / baseFrequency); //need 64-bit for high precision
		if (maxMs > 0xFFFFFFFF)
		{
			MessageLog<>::DefaultInstance().log<LogLevel::Error>(true,
				MessageLogPhrases::Text_PeriodicTriggerUnapproximatable,
				0xFFFFFFFF, '+',
				MessageLogPhrases::Text_UnitMicroseconds);
		}
		else
		{
			MessageLog<>::DefaultInstance().log<LogLevel::Error>(true,
				MessageLogPhrases::Text_PeriodicTriggerUnapproximatable,
				(uint32_t)maxMs,
				MessageLogPhrases::Text_UnitMicroseconds);
		}
		#endif
	}

	//setting period
	timerSteps -= setPeriod(timerSteps);

	//calculate actual period time in milliseconds
	return (int32_t)((int32_t)(((uint64_t)timerSteps * 1000000) / baseFrequency) - ms); //need temporary 64-bit for high precision
}

template <class Timer> int32_t PeriodicTrigger<Timer>::setPeriodHertz(uint32_t hz)
{
	uint32_t baseFrequency = timer->getBaseFrequency();
	uint32_t timerSteps;

	//This function doesn't work for asynchronous timer-mode!
	if (baseFrequency == 0 || hz == 0)
	{
		return -2147483648;
	}

	//Calculate needed number of timerSteps
	timerSteps = (baseFrequency / hz);
	if (timerSteps > getMaxPeriod()) //Will only happen on 8bit-Timers, so max-interval in terms of timersteps is known.
	{
		timerSteps = getMaxPeriod();
		#if ATMIGHTY_MESSAGELOG_ENABLE
		uint32_t maxHz = (baseFrequency / ((uint32_t)255 * 1024)) + 1;
		MessageLog<>::DefaultInstance().log<LogLevel::Error>(true,
				MessageLogPhrases::Text_PeriodicTriggerUnapproximatable,
				maxHz,
				MessageLogPhrases::Text_UnitHertz);
		#endif
	}

	//setting period
	timerSteps -= setPeriod(timerSteps);

	//calculate actual period time in milliseconds
	return (hz - (baseFrequency / timerSteps));
}


//Explizit template instantiations for this µC
template class PeriodicTrigger<Timer16bit>;
template class PeriodicTrigger<AbstractTimer16bit>;
template class PeriodicTrigger<Timer8bit>;
template class PeriodicTrigger<AbstractTimer8bit>;
template class PeriodicTrigger<AbstractTimer8bitAsync>;
#if defined (__AVR_ATmega2560__)
template class PeriodicTrigger<AbstractTimer0>;
template class PeriodicTrigger<AbstractTimer1>;
template class PeriodicTrigger<AbstractTimer2>;
template class PeriodicTrigger<AbstractTimer3>;
template class PeriodicTrigger<AbstractTimer4>;
template class PeriodicTrigger<AbstractTimer5>;
#endif
