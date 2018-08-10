/*!
 * Implements \see VirtualTimerPool
 */


#include "VirtualTimerPool.h"
#include <stdlib.h>
#include "ATmighty/Ressources/Interrupts/InterruptManager.h"
#include "ATmighty/Utilities/C++/FullCppSupport.h"
#include "Config/MessageLogConfig.h"
#include "ATmighty/Utilities/Logs/MessageLog.h"
#include "ATmighty/Utilities/LUTs/MessageLogPhrases.h"


template<class Timer> void VirtualTimerPool<Timer>::trigger()
{
	InterruptManager::enableNesting(); //enable nested interrupts as this might take long...
	for (uint8_t i = 0; i < poolsize; i++)
	{
		if (!(usageMap & (1 << i)))
		{
			if (typeMap & (1 << i)) //timer16bit
			{
				//timer16bit
			}
			else //timer8bit
			{
				vtimers[i].t8b.tick();
			}
		}
	}
}

template<class Timer> VirtualTimerPool<Timer>::VirtualTimerPool(uint16_t baseFrequency, Timer* sourceTimer, uint8_t poolsize)
: clock(PeriodicTrigger<Timer>(sourceTimer)),
  triggerFrequency(baseFrequency),
  vtimers(nullptr),
  usageMap((1 << poolsize) - 1),
  typeMap(0),
  poolsize(poolsize)
{
	if (poolsize < 1)
	{
		this->poolsize = 1;
	}
	else if (poolsize > 8)
	{
		this->poolsize = 8;
	}

	vtimers = (vtimer*)malloc(poolsize * sizeof(vtimer));
	if (vtimers != nullptr)
	{
		clock.setTriggerAction((Listener*)this);
		triggerFrequency -= clock.setPeriodHertz(baseFrequency);

		#if ATMIGHTY_MESSAGELOG_ENABLE
		MessageLog<>::DefaultInstance().log<LogLevel::Debug>(true,
				MessageLogPhrases::Text_VirtualTimerPoolSetup1,
				poolsize,
				MessageLogPhrases::Text_VirtualTimerPoolSetup2,
				triggerFrequency,
				MessageLogPhrases::Text_UnitHertz);
		#endif
	}
	else
	{
		#if ATMIGHTY_MESSAGELOG_ENABLE
		MessageLog<>::DefaultInstance().log<LogLevel::Fatal>(true,
				MessageLogPhrases::Text_VirtualTimerPoolSetupFail);
		#endif
	}
}

template<class Timer> VirtualTimerPool<Timer>::~VirtualTimerPool()
{
	clock.stop();
	clock.setTriggerAction((Listener*)nullptr);
	for (uint8_t i = 0; i < poolsize; i++)
	{
		if (usageMap & (1 << i))
		{
			if (typeMap & (1 << i)) //16-bit timer
			{

			}
			else //8-bit timer
			{
				vtimers[i].t8b.~VirtualTimer8bit(); //explicit destruction
			}
		}
	}
	free(vtimers);
}

template<class Timer> VirtualTimer8bit* VirtualTimerPool<Timer>::allocTimer8bit(uint8_t channels)
{
	if (usageMap != 0) //Any free places in this pool?
	{
		for (uint8_t i = 0; i < poolsize; i++)
		{
			if (usageMap & (1 << i)) //free place?
			{
				usageMap &= ~(1 << i);
				typeMap &= ~(1 << i);
				new (&(vtimers[i].t8b)) VirtualTimer8bit(triggerFrequency, channels); //Placement new
				vtimers[i].t8b.setVirtualTimerPoolIndex(i);

				#if ATMIGHTY_MESSAGELOG_ENABLE
				MessageLog<>::DefaultInstance().log<LogLevel::Info>(true,
						MessageLogPhrases::Text_VirtualTimerPoolVirtualized8bit,
						channels,
						MessageLogPhrases::Text_UnitChannels);
				#endif

				return &(vtimers[i].t8b);
			}
		}
	}

	#if ATMIGHTY_MESSAGELOG_ENABLE
	MessageLog<>::DefaultInstance().log<LogLevel::Error>(true,
			MessageLogPhrases::Text_VirtualTimerPoolVirtualizationFail);
	#endif

	return nullptr;
}

template<class Timer> VirtualTimer8bit* VirtualTimerPool<Timer>::allocTimer8bit(uint8_t channels, IoPin** outputPins)
{
	if ((usageMap != 0) && (outputPins != nullptr)) //Any free places in this pool and outputPins provided?
	{
		for (uint8_t i = 0; i < poolsize; i++)
		{
			if (usageMap & (1 << i)) //free place?
			{
				usageMap &= ~(1 << i);
				typeMap &= ~(1 << i);
				new (&(vtimers[i].t8b)) VirtualTimer8bit(triggerFrequency, outputPins, channels); //Placement new
				vtimers[i].t8b.setVirtualTimerPoolIndex(i);

				#if ATMIGHTY_MESSAGELOG_ENABLE
				MessageLog<>::DefaultInstance().log<LogLevel::Info>(true,
						MessageLogPhrases::Text_VirtualTimerPoolVirtualized8bit,
						channels,
						MessageLogPhrases::Text_UnitChannels);
				#endif

				return &(vtimers[i].t8b);
			}
		}
	}

	#if ATMIGHTY_MESSAGELOG_ENABLE
	MessageLog<>::DefaultInstance().log<LogLevel::Error>(true,
			MessageLogPhrases::Text_VirtualTimerPoolVirtualizationFail);
	#endif

	return nullptr;
}

template<class Timer> void VirtualTimerPool<Timer>::freeTimer(VirtualTimer8bit** timer)
{
	if ((timer != nullptr) && ((*timer) != nullptr))
	{
		uint8_t index = (*timer)->getVirtualTimerPoolIndex();

		if (&vtimers[index].t8b == (*timer)) //is timer actually the one from this pool?
		{
			vtimers[index].t8b.~VirtualTimer8bit(); //explicit destruction
			usageMap |= (1 << index);
			(*timer) = nullptr;

			#if ATMIGHTY_MESSAGELOG_ENABLE
			MessageLog<>::DefaultInstance().log<LogLevel::Info>(true,
					MessageLogPhrases::Text_VirtualTimerPoolFreed8bit);
			#endif
		}
		else
		{
			#if ATMIGHTY_MESSAGELOG_ENABLE
			MessageLog<>::DefaultInstance().log<LogLevel::Error>(true,
					MessageLogPhrases::Text_VirtualTimerPoolFreeFail);
			#endif
		}
	}
}


//Explizit template instantiations for this µC
template class VirtualTimerPool<Timer16bit>;
template class VirtualTimerPool<AbstractTimer16bit>;
//template class VirtualTimerPool<VirtualTimer16bit>;
template class VirtualTimerPool<Timer8bit>;
template class VirtualTimerPool<AbstractTimer8bit>;
template class VirtualTimerPool<AbstractTimer8bitAsync>;
template class VirtualTimerPool<VirtualTimer8bit>;
#if defined (__AVR_ATmega2560__)
template class VirtualTimerPool<AbstractTimer0>;
template class VirtualTimerPool<AbstractTimer1>;
template class VirtualTimerPool<AbstractTimer2>;
template class VirtualTimerPool<AbstractTimer3>;
template class VirtualTimerPool<AbstractTimer4>;
template class VirtualTimerPool<AbstractTimer5>;
#endif
