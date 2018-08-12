/*!
 * Implements \see StopWatch for the use with \see Abstract8bitTimer
 */

#include <ATmighty/Tools/Timing/Stopwatch/Stopwatch.h>
#include <util/atomic.h>
#include "ATmighty/Ressources/Periphery/Abstract/Timer.h"
#include "ATmighty/Ressources/Periphery/Virtual/Timer/VirtualTimer8bit.h"
#include "ATmighty/Utilities/Logs/MessageLog.h"
#include "ATmighty/Utilities/LUTs/MessageLogPhrases.h"
#include "Config/MessageLogConfig.h"


template<class Timer> void Stopwatch<Timer>::Overflower::trigger()
{
	overflows++;
}

template<class Timer> uint32_t Stopwatch<Timer>::Calibrate(Stopwatch<Timer>* reference)
{
	reference->start();
	uint32_t buf = reference->stop();
	return buf;
}

template<class Timer> Stopwatch<Timer>::Stopwatch(Timer* timer) : timer(timer), overflowCounter(), offset(0)
{
	//setup
	timer->stop();
	timer->setWGM(0);
	timer->enableTimerOverflowInterrupt(true);
	timer->setTimerOverflowISR(&overflowCounter);
	timer->setPrescalar(0); //run with cpu-frequency

	//calibration
	offset = (uint16_t)Stopwatch<Timer>::Calibrate(this);
	#if ATMIGHTY_MESSAGELOG_ENABLE
	MessageLog<>::DefaultInstance().log<LogLevel::Debug>(true, MessageLogPhrases::Text_StopwatchCalibration, (uint8_t)offset);
	#endif
}

template<class Timer> Stopwatch<Timer>::~Stopwatch()
{
	timer->enableTimerOverflowInterrupt(false);
	timer->setTimerOverflowISR((Listener*)nullptr);
}

template<class Timer> void Stopwatch<Timer>::start()
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		overflowCounter.reset();
		timer->setCounter(0);
		timer->enableTimerOverflowInterrupt(true); //clears pending interrupt
	}
}

template<class Timer> uint32_t Stopwatch<Timer>::stop()
{
	uint16_t overflows = overflowCounter.getOverflowCount();
	uint8_t timerCounter = timer->getCounter();
	uint32_t retBuf = ((uint32_t)timerCounter) + (uint32_t)(overflows * 0x100);

	if((overflowCounter.getOverflowCount() > overflows) && (timerCounter < 0x80)) //overflow must have occured just before reading timerCounter?
	{
		retBuf += 0x100;
	}

	return retBuf - offset;
}

//Explizit template instantiations for this µC
template class Stopwatch<Timer8bit>;
template class Stopwatch<AbstractTimer8bit>;
template class Stopwatch<VirtualTimer8bit>;
#if defined (__AVR_ATmega2560__)
template class Stopwatch<AbstractTimer0>;
template class Stopwatch<AbstractTimer2>;
#else
#  warning "No specialized StopWatch defined for this µC's 8-bit timers."
#endif
