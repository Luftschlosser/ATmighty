/*!
 * Implements \see TimeoutTrigger for 16bit-Timers
 */


#include "TimeoutTrigger.h"
#include "ATmighty/Ressources/Periphery/Abstract/Timer.h"
#include "ATmighty/Utilities/Logs/MessageLog.h"
#include "ATmighty/Utilities/LUTs/MessageLogPhrases.h"


template <class Timer> TimeoutTrigger<Timer>::TimeoutTrigger(Timer* timer) : timer(timer), triggerAction({nullptr}), calibrationOffset(0), statusFlags(0)
{
	if ((timer != nullptr) && (timer->getNumberOfChannels() > 0) && (timer->setWGM(4)/*CTC*/ == 0))
	{
		timer->enableOutputCompareInterrupt(false, 'A');
		timer->setOutputCompareISR((Listener*)this, 'A');
	}
	else
	{
		#if ATMIGHTY_MESSAGELOG_ENABLE
		MessageLog<>::DefaultInstance().log<LogLevel::Fatal>(true,
				MessageLogPhrases::Text_TimeoutTrigger,
				MessageLogPhrases::Text_TriggerInvalidTimer);
		#endif
	}
}

template <class Timer> int16_t TimeoutTrigger<Timer>::setTimespan(uint32_t timerSteps)
{
	if(!isRunning())
	{
		Timer16bit::Prescale scale;
		uint16_t scalefactor;
		uint32_t cntTop;
		int32_t retval;

		if (timerSteps <= calibrationOffset)
		{
			timerSteps = 1;
		}
		else
		{
			timerSteps -= calibrationOffset;
		}

		//find correct prescalar for the desired range
		if (timerSteps <= (uint32_t)0xFFFF)
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
			cntTop = 0xFFFF;
		}

		//set counter-top in timer
		timer->setOCRx((uint16_t)cntTop, 'A');
		#if ATMIGHTY_MESSAGELOG_ENABLE
		MessageLog<>::DefaultInstance().log<LogLevel::Debug>(true,
				MessageLogPhrases::Text_TimeoutTrigger,
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
					MessageLogPhrases::Text_TimeoutTriggerUnapproximatable,
					getMaxTimespan(),
					MessageLogPhrases::Text_UnitTimerSteps);
			#endif
			retval = -32768;
		}
		else if ((int16_t)retval != 0)
		{
			#if ATMIGHTY_MESSAGELOG_ENABLE
			MessageLog<>::DefaultInstance().log<LogLevel::Warning>(true,
					MessageLogPhrases::Text_TimeoutTriggerApproximated,
					(int16_t)retval,
					MessageLogPhrases::Text_UnitTimerSteps);
			#endif
		}

		//start counting with correct prescalar
		timer->setPrescalar(scale);

		//return
		return retval;
	}
	else
	{
		return 0;
	}
}

//Template-Specialization for Virtual 16bit-Timers (with more prescale-values)
//template <> int16_t TimeoutTrigger<VirtualTimer16bit>::setTimespan(uint32_t timerSteps) {}

template <class Timer> uint32_t TimeoutTrigger<Timer>::getMaxTimespan()
{
	return ((uint32_t)0xFFFF) * ((uint32_t)1024);
}


//Explizit template instantiations for this µC
template class TimeoutTrigger<Timer16bit>;
template class TimeoutTrigger<AbstractTimer16bit>;
//template class TimeoutTrigger<VirtualTimer16bit>;
#if defined (__AVR_ATmega2560__)
template class TimeoutTrigger<AbstractTimer1>;
template class TimeoutTrigger<AbstractTimer3>;
template class TimeoutTrigger<AbstractTimer4>;
template class TimeoutTrigger<AbstractTimer5>;
#else
#  warning "No specialized TimeoutTriggers defined for this µC's 16-bit timers."
#endif
