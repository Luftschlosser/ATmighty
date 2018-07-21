/*!
 * Implements \see VirtualTimerPool
 */


#include "VirtualTimerPool.h"
#include <stdlib.h>
#include "ATmighty/Ressources/Interrupts/InterruptManager.h"


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
	vtimers = (vtimer*)malloc(poolsize*sizeof(vtimer));
	if (vtimers != nullptr)
	{
		clock.setTriggerAction((Listener*)this);
		triggerFrequency -= clock.setPeriodHertz(baseFrequency);
	}
	else
	{
		//TODO log FATAL
	}
}

template<class Timer> VirtualTimerPool<Timer>::~VirtualTimerPool()
{
	clock.stop();
	clock.setTriggerAction((Listener*)nullptr);
	free(vtimers);
}

template<class Timer> VirtualTimer8bit* VirtualTimerPool<Timer>::allocTimer8bit(uint8_t channels)
{
	if (usageMap != 0) //Any free places in this pool?
	{
		for (uint8_t i = 0; i < poolsize; i++)
		{
			if (usageMap & (1 << i))
			{
				usageMap &= ~(1 << i);
				typeMap &= ~(1 << i);
				vtimers[i].t8b = VirtualTimer8bit(triggerFrequency, channels);
				vtimers[i].t8b.setVirtualTimerPoolIndex(i);
				return &(vtimers[i].t8b);
			}
		}
	}
	return nullptr;
}

template<class Timer> VirtualTimer8bit* VirtualTimerPool<Timer>::allocTimer8bit(uint8_t channels, IoPin** outputPins)
{
	if ((usageMap != 0) && (outputPins != nullptr)) //Any free places in this pool and outputPins provided?
	{
		for (uint8_t i = 0; i < poolsize; i++)
		{
			if (usageMap & (1 << i))
			{
				usageMap &= ~(1 << i);
				typeMap &= ~(1 << i);
				vtimers[i].t8b = VirtualTimer8bit(triggerFrequency, outputPins, channels);
				vtimers[i].t8b.setVirtualTimerPoolIndex(i);
				return &(vtimers[i].t8b);
			}
		}
	}
	return nullptr;
}

template<class Timer> void VirtualTimerPool<Timer>::freeTimer(VirtualTimer8bit** timer)
{
	if ((timer != nullptr) && ((*timer) != nullptr))
	{
		uint8_t index = (*timer)->getVirtualTimerPoolIndex();

		if (&vtimers[index].t8b == (*timer)) //is timer actually the one from this pool?
		{
			vtimers[index].t8b.~VirtualTimer8bit(); //destruct object manually.
			usageMap |= (1 << index);
			(*timer) = nullptr;
		}
		//Todo: else log error
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
