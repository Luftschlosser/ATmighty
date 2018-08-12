/*!
 * Implements \see PeriodicTrigger-functions which are timer-independent
 */


#include "PeriodicTrigger.h"
#include "ATmighty/Tools/Timing/TimeUnitConverter/TimeUnitConverter.h"
#include "ATmighty/Ressources/Periphery/Abstract/Timer.h"
#include "ATmighty/Ressources/Periphery/Virtual/Timer/VirtualTimer8bit.h"
#include "ATmighty/Utilities/Logs/MessageLog.h"
#include "ATmighty/Utilities/LUTs/MessageLogPhrases.h"
#include "Config/MessageLogConfig.h"


template <class Timer> PeriodicTrigger<Timer>::~PeriodicTrigger()
{
	timer->stop();
	stop();
	setTriggerAction((Listener*)nullptr);
}

template <class Timer> int16_t PeriodicTrigger<Timer>::setPeriodSeconds(uint16_t s)
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

	if (timerSteps > getMaxPeriod())
	{
		timerSteps = getMaxPeriod();
		#if ATMIGHTY_MESSAGELOG_ENABLE
		MessageLog<>::DefaultInstance().log<LogLevel::Error>(true,
				MessageLogPhrases::Text_PeriodicTriggerUnapproximatable,
				converter.cyclesToSeconds(timerSteps),
				MessageLogPhrases::Text_UnitSeconds);
		#endif
	}

	//setting period
	timerSteps -= setPeriod(timerSteps);

	//calculate actual period time in seconds
	return (int16_t)((int16_t)converter.cyclesToSeconds(timerSteps) - s);
}

template <class Timer> int16_t PeriodicTrigger<Timer>::setPeriodMilliseconds(uint16_t ms)
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

	if (timerSteps > getMaxPeriod())
	{
		timerSteps = getMaxPeriod();
		#if ATMIGHTY_MESSAGELOG_ENABLE
		MessageLog<>::DefaultInstance().log<LogLevel::Error>(true,
				MessageLogPhrases::Text_PeriodicTriggerUnapproximatable,
				converter.cyclesToMilliseconds(timerSteps),
				MessageLogPhrases::Text_UnitMilliseconds);
		#endif
	}

	//setting period
	timerSteps -= setPeriod(timerSteps);

	//calculate actual period time in milliseconds
	return (int16_t)((int16_t)converter.cyclesToMilliseconds(timerSteps) - ms);
}

template <class Timer> int32_t PeriodicTrigger<Timer>::setPeriodMicroseconds(uint32_t ms)
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

	if (timerSteps > getMaxPeriod())
	{
		timerSteps = getMaxPeriod();
		#if ATMIGHTY_MESSAGELOG_ENABLE
		uint64_t maxMs = converter.cyclesToMicroseconds(timerSteps);
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

	//calculate actual period time in microseconds
	return (int32_t)((int32_t)converter.cyclesToMicroseconds(timerSteps) - ms); //need temporary 64-bit for high precision
}

template <class Timer> int32_t PeriodicTrigger<Timer>::setPeriodHertz(uint32_t hz)
{
	TimeUnitConverter<uint32_t> converter(timer->getBaseFrequency());
	uint32_t timerSteps;

	//This function doesn't work for asynchronous timer-mode!
	if (converter.getBaseFrequency() == 0 || hz == 0)
	{
		return -2147483648;
	}

	//Calculate needed number of timerSteps
	timerSteps = converter.hertzToCycles(hz);
	if (timerSteps > getMaxPeriod()) //Will only happen on 8bit-Timers, so max-interval in terms of timersteps is known.
	{
		timerSteps = getMaxPeriod();
		#if ATMIGHTY_MESSAGELOG_ENABLE
		uint32_t maxHz = converter.cyclesToHertz((uint32_t)255 * 1024) + 1;
		MessageLog<>::DefaultInstance().log<LogLevel::Error>(true,
				MessageLogPhrases::Text_PeriodicTriggerUnapproximatable,
				maxHz,
				MessageLogPhrases::Text_UnitHertz);
		#endif
	}

	//setting period
	timerSteps -= setPeriod(timerSteps);

	//calculate actual period time in hertz
	return (hz - converter.cyclesToHertz(timerSteps));
}


//Explizit template instantiations for this µC
template class PeriodicTrigger<Timer16bit>;
template class PeriodicTrigger<AbstractTimer16bit>;
template class PeriodicTrigger<Timer8bit>;
template class PeriodicTrigger<AbstractTimer8bit>;
template class PeriodicTrigger<AbstractTimer8bitAsync>;
template class PeriodicTrigger<VirtualTimer8bit>;
#if defined (__AVR_ATmega2560__)
template class PeriodicTrigger<AbstractTimer0>;
template class PeriodicTrigger<AbstractTimer1>;
template class PeriodicTrigger<AbstractTimer2>;
template class PeriodicTrigger<AbstractTimer3>;
template class PeriodicTrigger<AbstractTimer4>;
template class PeriodicTrigger<AbstractTimer5>;
#else
#  warning "No specialized PeriodicTriggers defined for this µC."
#endif
