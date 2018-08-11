/*!
 * Implements \see PeriodicTrigger for 8bit-Timers
 */


#include <ATmighty/Tools/Timing/PeriodicTrigger/PeriodicTrigger.h>
#include "ATmighty/Ressources/Periphery/Abstract/Timer.h"
#include "ATmighty/Ressources/Periphery/Virtual/Timer/VirtualTimer8bit.h"
#include "ATmighty/Utilities/Logs/MessageLog.h"
#include "ATmighty/Utilities/LUTs/MessageLogPhrases.h"
#include "Config/MessageLogConfig.h"


template <class Timer> PeriodicTrigger<Timer>::PeriodicTrigger(Timer* timer) : timer(timer)
{
	if ((timer->getNumberOfChannels() < 1) || (timer->setWGM(2) != 0)) //no channelA or no CTC-Mode?
	{
		#if ATMIGHTY_MESSAGELOG_ENABLE
		MessageLog<>::DefaultInstance().log<LogLevel::Fatal>(true,
				MessageLogPhrases::Text_PeriodicTrigger,
				MessageLogPhrases::Text_TriggerInvalidTimer);
		#endif
	}
}

//General Implementation for normal 8bit-Timers
template <class Timer> int16_t PeriodicTrigger<Timer>::setPeriod(uint32_t timerSteps)
{
	Timer8bit::Prescale scale;
	uint16_t scalefactor;
	uint32_t cntTop;
	int32_t retval;

	//find correct prescalar for the desired range
	if (timerSteps <= (uint32_t)0xFF)
	{
		scale = Timer8bit::Prescale::NoScale;
		if (timerSteps == 0)
		{
			timerSteps++;
		}
	}
	else if (timerSteps <= ((uint32_t)0xFF * 8))
	{
		scale = Timer8bit::Prescale::Scale8;
	}
	else if (timerSteps <= ((uint32_t)0xFF * 64))
	{
		scale = Timer8bit::Prescale::Scale64;
	}
	else if (timerSteps <= ((uint32_t)0xFF * 256))
	{
		scale = Timer8bit::Prescale::Scale256;
	}
	else
	{
		scale = Timer8bit::Prescale::Scale1024;
	}
	scalefactor = (1 << (uint8_t)scale);

	//calculate counterTop for chosen prescalar
	cntTop = timerSteps >> (uint8_t)scale;

	//round counterTop up?
	if ((timerSteps % scalefactor) > (scalefactor >> 1))
	{
		cntTop++;
	}

	//round counter down to maximal 8bit-value
	if (cntTop > 0xFF)
	{
		cntTop = 0x100;
	}

	//set counter-top in timer
	timer->setOCRx((uint8_t)((uint16_t)cntTop - 1), 'A');
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
	return retval;
}

//Template-Specialization for Virtual 8bit-Timers (with more prescale-values)
template <> int16_t PeriodicTrigger<VirtualTimer8bit>::setPeriod(uint32_t timerSteps)
{
	uint8_t scale;
	uint16_t scalefactor;
	uint32_t cntTop;
	int32_t retval;

	if (timerSteps == 0)
	{
		timerSteps++;
	}

	//find correct prescalar for the desired range
	for (scale = 0; scale < 16; scale++)
	{
		if (timerSteps <= ((uint32_t)0xFF * (1 << scale)))
		{
			break;
		}
	}
	if (scale > 15)
	{
		scale = 15;
	}
	scalefactor = (1 << scale);

	//calculate counterTop for chosen prescalar
	cntTop = timerSteps >> scale;

	//round counterTop up?
	if ((timerSteps % scalefactor) > (scalefactor >> 1))
	{
		cntTop++;
	}

	//round counter down to maximal 8bit-value
	if (cntTop > 0xFF)
	{
		cntTop = 0x100;
	}

	//set counter-top in timer
	timer->setOCRx((uint8_t)((uint16_t)cntTop - 1), 'A');
	#if ATMIGHTY_MESSAGELOG_ENABLE
	MessageLog<>::DefaultInstance().log<LogLevel::Debug>(true,
			MessageLogPhrases::Text_PeriodicTrigger,
			MessageLogPhrases::Text_TriggerValuesSet,
			scalefactor, '/', cntTop);
	#endif

	//calculate return value
	cntTop = cntTop << scale;
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
	return retval;
}

template <class Timer> uint32_t PeriodicTrigger<Timer>::getMaxPeriod()
{
	return ((uint32_t)0xFF) * ((uint32_t)1024);
}


//Explizit template instantiations for this µC
template class PeriodicTrigger<Timer8bit>;
template class PeriodicTrigger<AbstractTimer8bit>;
template class PeriodicTrigger<VirtualTimer8bit>;
#if defined (__AVR_ATmega2560__)
template class PeriodicTrigger<AbstractTimer0>;
#endif
