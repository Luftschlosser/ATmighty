/*!
 * Implements \see TimeoutTrigger for asynchronous 8bit-Timers (as these have additional prescale-values)
 */


#include "TimeoutTrigger.h"
#include "ATmighty/Ressources/Periphery/Abstract/Timer.h"
#include "ATmighty/Utilities/Logs/MessageLog.h"
#include "ATmighty/Utilities/LUTs/MessageLogPhrases.h"


template <class Timer> TimeoutTrigger<Timer>::TimeoutTrigger(Timer* timer) : timer(timer), triggerAction({nullptr}), calibrationOffset(0), statusFlags(0)
{
	if ((timer != nullptr) && (timer->getNumberOfChannels() > 0) && (timer->setWGM(2)/*CTC*/ == 0))
	{
		timer->enableOutputCompareInterrupt(false, 'A');
		timer->setOutputCompareISR((Listener*)this, 'A');
	}
	else
	{
		//todo error
	}
}

template <class Timer> int16_t TimeoutTrigger<Timer>::setTimespan(uint32_t timerSteps)
{
	if(!isRunning())
	{
		Timer8bit::Prescale scale;
		uint16_t scalefactor;
		uint32_t cntTop;
		int32_t retval;

		if (timerSteps < (calibrationOffset + 30))
		{
			timerSteps = 30; //use this as arbitrary minimum
		}
		else
		{
			timerSteps -= calibrationOffset;
		}

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
		else if (timerSteps <= ((uint32_t)0xFF * 32))
		{
			scale = Timer8bit::Prescale::Scale32;
		}
		else if (timerSteps <= ((uint32_t)0xFF * 64))
		{
			scale = Timer8bit::Prescale::Scale64;
		}
		else if (timerSteps <= ((uint32_t)0xFF * 128))
		{
			scale = Timer8bit::Prescale::Scale128;
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
			cntTop = 0xFF;
		}

		//set counter-top in timer
		timer->setOCRx((uint8_t)cntTop, 'A');
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

template <class Timer> uint32_t TimeoutTrigger<Timer>::getMaxTimespan()
{
	return ((uint32_t)0xFF) * ((uint32_t)1024);
}


//Explizit template instantiations for this µC
template class TimeoutTrigger<AbstractTimer8bitAsync>;
#if defined (__AVR_ATmega2560__)
template class TimeoutTrigger<AbstractTimer2>;
#endif
