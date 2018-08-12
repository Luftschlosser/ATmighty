/*
 * Implements \see VirtualTimer8bit
 */


#include "VirtualTimer8bit.h"
#include <stdlib.h>
#include "ATmighty/Ressources/Periphery/Abstract/IoPins.h"
#include "ATmighty/Interfaces/Listener.h"
#include "Config/MessageLogConfig.h"
#include "ATmighty/Utilities/Logs/MessageLog.h"
#include "ATmighty/Utilities/LUTs/MessageLogPhrases.h"


VirtualTimer8bit::VirtualTimer8bit(uint16_t baseFrequency, IoPin** outputs, uint8_t channels)
: channelData(nullptr),
  baseFrequency(baseFrequency),
  tcnt(0),
  wgm(0),
  prescalar(0),
  prescaleCounter(0),
  comx(0),
  channels(channels),
  interruptEnFlags(0),
  isrTypeMask(0),
  timerOverflowISR({nullptr}),
  outputPins(outputs),
  statusFlags(0)
{
	if (channels == 0)
	{
		this->channels = 1;
	}
	else if (channels > 4)
	{
		this->channels = 4;

		#if ATMIGHTY_MESSAGELOG_ENABLE
		MessageLog<>::DefaultInstance().log<LogLevel::Warning>(true,
				MessageLogPhrases::Text_VirtualTimerChannelMax);
		#endif
	}
	channelData = (channelData_t*)malloc(this->channels * sizeof(channelData));

	if (channelData == nullptr)
	{
		#if ATMIGHTY_MESSAGELOG_ENABLE
		MessageLog<>::DefaultInstance().log<LogLevel::Fatal>(true,
				MessageLogPhrases::Text_VirtualTimerSetupFail);
		#endif
	}
}

VirtualTimer8bit::VirtualTimer8bit(uint16_t baseFrequency, uint8_t channels)
: VirtualTimer8bit(baseFrequency, nullptr, channels)
{
}

VirtualTimer8bit::~VirtualTimer8bit()
{
	free(channelData);
}

void VirtualTimer8bit::tick()
{
	if (statusFlags & 1) //timer running?
	{
		prescaleCounter++;
		if ((prescaleCounter >= prescalar) || (statusFlags & (1 << 4))) //actual prescaled tick? or BypassPrescalar?
		{
			//resetting prescaleCounter XOR clearing BypassPrescalar bit
			if (statusFlags & (1 << 4))
			{
				statusFlags &= ~(1 << 4);
			}
			else
			{
				prescaleCounter = 0;
			}

			//counting
			if (!(statusFlags & (1 << 3))) //not skipNextCount?
			{
				if (statusFlags & (1 << 1)) //decrementing?
				{
					tcnt--;
					if (tcnt == 0)
					{
						statusFlags &= ~(1 << 1); //switch to incrementing
					}
				}
				else
				{
					tcnt++;
					if ((tcnt == 0xFF) && (wgm == 1))
					{
						statusFlags |= (1 << 1); //switch to decrementing in Phase-correct PWM-Mode with Top=0xFF
					}
				}
			}
			else
			{
				statusFlags &= ~(1 << 3); // clear bit skipNextCount
			}

			//Fast-Pwm-Bottom events
			if ((tcnt == 0) && ((wgm == 3) || (wgm == 7)))
			{
				for (uint8_t i = 0; i < channels; i++)
				{
					uint8_t com = ((comx >> (i * 2)) & 3);
					if (com == 2)
					{
						outputPins[i]->set(true);
					}
					else if (com == 3)
					{
						outputPins[i]->set(false);
					}
				}
			}

			//TOV-Events
			if (tcnt == 0 && ((wgm == 1) || (wgm == 5)))
			{
				timerOverflowEvent();
			}
			else if (tcnt == 0xFF && ((wgm == 0) || (wgm == 2) || (wgm == 3)))
			{
				timerOverflowEvent();
			}

			//Compare-Match-Events 1-n
			for (uint8_t i = 1; i < channels; i++)
			{
				if (tcnt == channelData[i].ocrx)
				{
					compareMatchEvent(i);
				}
			}

			//Compare-Match-Event 0
			if (tcnt == channelData[0].ocrx)
			{
				if (wgm == 2)
				{
					tcnt = 0;
					statusFlags |= (1 << 3); //skipNextCount
				}
				else if (wgm == 5)
				{
					statusFlags |= (1 << 1); //switch to decrement in Phase-correct PWM-Mode with Top=OCR0
					updateOCRx(); //update OCRx for wgm5
				}
				else if (wgm == 7)
				{
					tcnt = 0;
					timerOverflowEvent();
					statusFlags |= (1 << 3); //skipNextCount
				}
				compareMatchEvent(0);
			}

			//Other OCRx-Update events
			if (((tcnt == 0xFF) && (wgm < 4)) || ((tcnt == 0) && (wgm == 7)))
			{
				updateOCRx();
			}
		}
	}
}

void VirtualTimer8bit::compareMatchEvent(uint8_t channel)
{
	//Outputs
	uint8_t com = ((comx >> (channel * 2)) & 3);

	if (com != 0)
	{
		if ((wgm == 0) || (wgm == 2)) //non-PWM-mode?
		{
			if (outputPins != nullptr) //outputPins set?
			{
				if (com == 1)
				{
					outputPins[channel]->toggle();
				}
				else if (com == 2)
				{
					outputPins[channel]->set(false);
				}
				else if (com == 3)
				{
					outputPins[channel]->set(true);
				}
			}
		}
		else if (com > 1)
		{
			if ((wgm == 3) || (wgm == 7)) //fast PWM-Mode
			{
				if (com == 2)
				{
					outputPins[channel]->set(false);
				}
				else //com == 3
				{
					outputPins[channel]->set(true);
				}
			}
			else //Phase correct PWM-Mode
			{
				if ((statusFlags & (1 << 1)) ^ (com == 2)) //counting down XOR Compare-Mode 2
				{
					outputPins[channel]->set(false);
				}
				else //Counting up XOR Compare-Mode 3
				{
					outputPins[channel]->set(true);
				}
			}
		}
	}

	//Interrupts
	if ((interruptEnFlags & (1 << channel)) && (channelData[channel].outputCompareMatchISR.callback != nullptr))
	{
		if (isrTypeMask & (1 << channel))
		{
			channelData[channel].outputCompareMatchISR.listener->trigger();
		}
		else
		{
			channelData[channel].outputCompareMatchISR.callback();
		}
	}
}

void VirtualTimer8bit::timerOverflowEvent()
{
	if ((interruptEnFlags & (1 << 7)) && (timerOverflowISR.callback != nullptr))
	{
		if (isrTypeMask & (1 << 7))
		{
			timerOverflowISR.listener->trigger();
		}
		else
		{
			timerOverflowISR.callback();
		}
	}
}

void VirtualTimer8bit::updateOCRx()
{
	if (statusFlags & (1 << 2))
	{
		for (uint8_t i = 0; i < channels; i++)
		{
			channelData[i].ocrx = channelData[i].ocrxBuffer;
		}
		statusFlags &= ~(1 << 2);
	}
}

int8_t VirtualTimer8bit::setWGM(uint8_t value)
{
	if ((value > 7) || (value == 4) || (value == 6))
	{
		return -1;
	}
	else
	{
		wgm = value;
		return 0;
	}
}

int8_t VirtualTimer8bit::setPrescalar(uint8_t potency)
{
	if (potency >= 16)
	{
		return -1;
	}
	else
	{
		prescalar = (1 << potency);

		#if ATMIGHTY_MESSAGELOG_ENABLE
		if (!(statusFlags & 1))
		{
			MessageLog<>::DefaultInstance().log<LogLevel::Debug>(true,
					MessageLogPhrases::Text_VirtualTimerStarted1,
					prescalar,
					MessageLogPhrases::Text_VirtualTimerStarted2,
					wgm,
					MessageLogPhrases::Text_VirtualTimerStarted3,
					baseFrequency,
					MessageLogPhrases::Text_UnitHertz);
		}
		#endif

		statusFlags |= 1; //Set timer-state "running"
		return 0;
	}
}

int8_t VirtualTimer8bit::setOCRx(uint8_t value, char channel)
{
	uint8_t channelIndex = (uint8_t)(channel - 'A');
	if (channelIndex < channels)
	{
		channelData[channelIndex].ocrxBuffer = value;
		if ((wgm == 0) || (wgm == 2)) //non-pwm-mode
		{
			channelData[channelIndex].ocrx = value;
		}
		else //pwm-mode, buffer value to be set later
		{
			statusFlags |= (1 << 2);
		}
		return 0;
	}
	else
	{
		return 1;
	}
}

int8_t VirtualTimer8bit::forceOutputCompare(char channel)
{
	uint8_t channelIndex = (uint8_t)(channel - 'A');
	if (channelIndex < channels) //valid channel
	{
		if ((wgm == 0) || (wgm == 2)) //non-PWM-mode?
		{
			uint8_t com = ((comx >> (channel * 2)) & 3);
			if (outputPins != nullptr) //outputPins set?
			{
				if (com == 1)
				{
					outputPins[channelIndex]->toggle();
				}
				else if (com == 2)
				{
					outputPins[channelIndex]->set(false);
				}
				else if (com == 3)
				{
					outputPins[channelIndex]->set(true);
				}
			}
			return 0;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return 1;
	}
}

int8_t VirtualTimer8bit::setCOMx(uint8_t value, char channel)
{
	if (outputPins != nullptr) //outputPins provided?
	{
		if (value <= 3) //value valid?
		{
			uint8_t channelIndex = (uint8_t)(channel - 'A');
			if (channelIndex < channels) //valid channel?
			{
				outputPins[channelIndex]->setDirection(IoPin::DataDirection::Output);
				outputPins[channelIndex]->set(false);
				channelIndex *= 2;
				comx = ((comx & ~(3 << channelIndex)) | (value << channelIndex));
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return 2;
	}
}

IoPin* VirtualTimer8bit::getOutputPin(char channel)
{
	uint8_t channelIndex = (uint8_t)(channel - 'A');
	if ((outputPins != nullptr) && (channelIndex < channels))
	{
		return outputPins[channelIndex];
	}
	else
	{
		return nullptr;
	}
}

void VirtualTimer8bit::enableOutputCompareInterrupt(bool value, char channel)
{
	uint8_t channelIndex = (uint8_t)(channel - 'A');
	if (channelIndex < channels)
	{
		if (value)
		{
			interruptEnFlags |= (1 << channelIndex);
		}
		else
		{
			interruptEnFlags &= ~(1 << channelIndex);
		}
	}
}

void VirtualTimer8bit::enableTimerOverflowInterrupt(bool value)
{
	if (value)
	{
		interruptEnFlags |= (1 << 7);
	}
	else
	{
		interruptEnFlags &= ~(1 << 7);
	}
}

void VirtualTimer8bit::setOutputCompareISR(Listener* isr, char channel)
{
	uint8_t channelIndex = (uint8_t)(channel - 'A');
	if (channelIndex < channels)
	{
		isrTypeMask |= (1 << channelIndex);
		channelData[channelIndex].outputCompareMatchISR.listener = isr;
	}
}

void VirtualTimer8bit::setOutputCompareISR(void (*isr)(), char channel)
{
	uint8_t channelIndex = (uint8_t)(channel - 'A');
	if (channelIndex < channels)
	{
		isrTypeMask &= ~(1 << channelIndex);
		channelData[channelIndex].outputCompareMatchISR.callback = isr;
	}
}

void VirtualTimer8bit::setTimerOverflowISR(Listener* isr)
{
	isrTypeMask |= (1 << 7);
	timerOverflowISR.listener = isr;
}

void VirtualTimer8bit::setTimerOverflowISR(void (*isr)())
{
	isrTypeMask &= ~(1 << 7);
	timerOverflowISR.callback = isr;
}
