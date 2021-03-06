/*!
 * Implements \see PeriodicTrigger for 16bit-Timers
 */


#include <ATmighty/Tools/Timing/PeriodicTrigger/PeriodicTrigger.h>
#include "ATmighty/Ressources/Periphery/Abstract/Timer.h"
#include "ATmighty/Utilities/Logs/MessageLog.h"
#include "ATmighty/Utilities/LUTs/MessageLogPhrases.h"
#include "Config/MessageLogConfig.h"


template <class Timer> PeriodicTrigger<Timer>::PeriodicTrigger(Timer* timer) : timer(timer)
{
	if ((timer->getNumberOfChannels() < 1) || (timer->setWGM(4) != 0)) //no channelA or no CTC-Mode?
	{
		#if ATMIGHTY_MESSAGELOG_ENABLE
		MessageLog<>::DefaultInstance().log<LogLevel::Fatal>(true,
				MessageLogPhrases::Text_PeriodicTrigger,
				MessageLogPhrases::Text_TriggerInvalidTimer);
		#endif
	}
}

template <class Timer> int16_t PeriodicTrigger<Timer>::setPeriod(uint32_t timerSteps)
{
	Timer16bit::Prescale scale;
	uint16_t scalefactor;
	uint32_t cntTop;
	int32_t retval;

	//find correct prescalar for the desired range
	if (timerSteps <= 0xFFFF)
	{
		scale = Timer16bit::Prescale::NoScale;
		if (timerSteps == 0)
		{
			timerSteps++;
		}
	}
	else if (timerSteps <= ((uint32_t)0xFFFF * 8))
	{
		scale = Timer16bit::Prescale::Scale8;
	}
	else if (timerSteps <= ((uint32_t)0xFFFF * 64))
	{
		scale = Timer16bit::Prescale::Scale64;
	}
	else if (timerSteps <= ((uint32_t)0xFFFF * 256))
	{
		scale = Timer16bit::Prescale::Scale256;
	}
	else
	{
		scale = Timer16bit::Prescale::Scale1024;
	}
	scalefactor = (1 << (uint8_t)scale);

	//calculate counterTop for chosen prescalar
	cntTop = timerSteps >> (uint8_t)scale;

	//round counterTop up?
	if ((timerSteps % scalefactor) > (scalefactor >> 1))
	{
		cntTop++;
	}

	//round counter down to maximal 16bit-value
	if (cntTop > 0xFFFF)
	{
		cntTop = 0x10000;
	}

	//set counter-top in timer
	timer->setOCRx((uint16_t)(cntTop - 1), 'A');
	#if ATMIGHTY_MESSAGELOG_ENABLE
	MessageLog<>::DefaultInstance().log<LogLevel::Debug>(true,
			MessageLogPhrases::Text_PeriodicTrigger,
			MessageLogPhrases::Text_TriggerValuesSet,
			scalefactor, '/', cntTop);
	#endif
	
	//calculate return value
	cntTop = cntTop << (uint8_t)scale;
	retval = cntTop - timerSteps;
	if (retval < -32768)
	{
		#if ATMIGHTY_MESSAGELOG_ENABLE
		MessageLog<>::DefaultInstance().log<LogLevel::Error>(true,
				MessageLogPhrases::Text_PeriodicTriggerUnapproximatable,
				getMaxPeriod(),
				MessageLogPhrases::Text_UnitTimerSteps);
		#endif
		retval = -32768;
	}
	else if ((int16_t)retval != 0)
	{
		#if ATMIGHTY_MESSAGELOG_ENABLE
		MessageLog<>::DefaultInstance().log<LogLevel::Warning>(true,
				MessageLogPhrases::Text_PeriodicTriggerApproximated,
				(int16_t)retval,
				MessageLogPhrases::Text_UnitTimerSteps);
		#endif
	}

	//start counting with correct prescalar
	timer->setPrescalar(scale);

	//return
	return (int16_t)retval;
}

template <class Timer> uint32_t PeriodicTrigger<Timer>::getMaxPeriod()
{
	return ((uint32_t)0xFFFF) * ((uint32_t)1024);
}


//Explizit template instantiations for this µC
template class PeriodicTrigger<Timer16bit>;
template class PeriodicTrigger<AbstractTimer16bit>;
#if defined (__AVR_ATmega2560__)
template class PeriodicTrigger<AbstractTimer1>;
template class PeriodicTrigger<AbstractTimer3>;
template class PeriodicTrigger<AbstractTimer4>;
template class PeriodicTrigger<AbstractTimer5>;
#else
#  warning "No specialized PeriodicTriggers defined for this µC's 16-bit timers."
#endif
