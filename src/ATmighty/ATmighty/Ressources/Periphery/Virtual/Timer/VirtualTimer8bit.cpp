/*
 * Implements \see VirtualTimer8bit
 */


#include "VirtualTimer8bit.h"
#include <stdlib.h>
#include "ATmighty/Ressources/Periphery/Abstract/IoPins.h"
#include "ATmighty/Interfaces/Listener.h"
#include "ATmighty/Utilities/Logs/MessageLog.h"


VirtualTimer8bit::VirtualTimer8bit(uint16_t baseFrequency, IoPin** outputs, uint8_t channels)
: baseFrequency(baseFrequency),
  tcnt(0),
  wgm(0),
  prescalar(0),
  prescaleCounter(0),
  ocrx(nullptr),
  ocrxBuffers(nullptr),
  comx(0),
  channels(channels),
  interruptEnFlags(0),
  isrTypeMask(0),
  outputCompareMatchISRs(nullptr),
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
		//Todo log warning
	}
	ocrx = (uint8_t*)malloc(this->channels * sizeof(uint8_t));
	ocrxBuffers = (uint8_t*)malloc(this->channels * sizeof(uint8_t));
	outputCompareMatchISRs = (isr_t*)malloc(this->channels * sizeof(isr_t));

	if ((ocrx == nullptr) || (ocrxBuffers == nullptr) || (outputCompareMatchISRs == nullptr))
	{
		//Todo: change implementation
		MessageLog<>::DefaultInstance().log<LogLevel::Fatal>(false, "malloc failed!");
	}
}

VirtualTimer8bit::VirtualTimer8bit(uint16_t baseFrequency, uint8_t channels)
: VirtualTimer8bit(baseFrequency, nullptr, channels)
{
}

VirtualTimer8bit::~VirtualTimer8bit()
{
	free(ocrx);
	free(ocrxBuffers);
	free(outputCompareMatchISRs);
}

void VirtualTimer8bit::tick()
{
	if (statusFlags & 1) //timer running?
	{
		prescaleCounter++;
		if (prescaleCounter >= prescalar) //actual prescaled tick?
		{
			static bool skipNextCount = false;

			//resetting prescaleCounter
			prescaleCounter = 0;

			//counting
			if (!skipNextCount)
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
				skipNextCount = false;
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
				if (tcnt == ocrx[i])
				{
					compareMatchEvent(i);
				}
			}

			//Compare-Match-Event 0
			if (tcnt == ocrx[0])
			{
				compareMatchEvent(0);
				if (wgm == 2)
				{
					tcnt = 0;
					skipNextCount = true;
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
					skipNextCount = true;
				}
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
	if ((interruptEnFlags & (1 << channel)) && (outputCompareMatchISRs[channel].callback != nullptr))
	{
		if (isrTypeMask & (1 << channel))
		{
			outputCompareMatchISRs[channel].listener->trigger();
		}
		else
		{
			outputCompareMatchISRs[channel].callback();
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
			ocrx[i] = ocrxBuffers[i];
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
		statusFlags |= 1; //Set timer-state "running"
		return 0;
	}
}

int8_t VirtualTimer8bit::setOCRx(uint8_t value, char channel)
{
	uint8_t channelIndex = (uint8_t)(channel - 'A');
	if (channelIndex < channels)
	{
		ocrxBuffers[channelIndex] = value;
		if ((wgm == 0) || (wgm == 2)) //non-pwm-mode
		{
			ocrx[channelIndex] = value;
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
		outputCompareMatchISRs[channelIndex].listener = isr;
	}
}

void VirtualTimer8bit::setOutputCompareISR(void (*isr)(), char channel)
{
	uint8_t channelIndex = (uint8_t)(channel - 'A');
	if (channelIndex < channels)
	{
		isrTypeMask &= ~(1 << channelIndex);
		outputCompareMatchISRs[channelIndex].callback = isr;
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
