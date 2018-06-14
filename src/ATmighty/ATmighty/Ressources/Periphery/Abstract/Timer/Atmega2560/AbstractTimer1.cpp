/*!
 * This file implements class \see AbstractTimer1
 */


#include "AbstractTimer1.h"
#include <avr/io.h>
#include <util/atomic.h>
#include <avr/interrupt.h>
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"
#include "ATmighty/Ressources/Periphery/Abstract/AbstractHardwareManager.h"
#include "ATmighty/Utilities/LUTs/HardwareOwnerID.h"
#include "ATmighty/Interfaces/Listener.h"
#include "ATmighty/Ressources/Interrupts/InterruptManager.h"


//AbstractTimer1-class implementation

int8_t AbstractTimer1::Owner = 0;
SpecificIoPin<'B',5>* AbstractTimer1::OutputA = nullptr;
SpecificIoPin<'B',6>* AbstractTimer1::OutputB = nullptr;
SpecificIoPin<'B',7>* AbstractTimer1::OutputC = nullptr;
SpecificIoPin<'D',6>* AbstractTimer1::ExtClk = nullptr;
SpecificIoPin<'D',4>* AbstractTimer1::Icp = nullptr;

int8_t AbstractTimer1::init(int8_t ownerId)
{
	if (Owner == 0)
	{
		physicalReference = PhysicalHardwareManager::Alloc<Timer1>(OwnerID::DirectAbstraction);

		if (physicalReference != nullptr)
		{
			Owner = ownerId;
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

void AbstractTimer1::exit()
{
	PhysicalHardwareManager::Free(&physicalReference);
	if (OutputA || OutputB || OutputC ||OutputB || Icp)
	{
		AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);
		if (OutputA != nullptr)
		{
			hwManager.freeItem(&OutputA);
		}
		if (OutputB != nullptr)
		{
			hwManager.freeItem(&OutputB);
		}
		if (OutputC != nullptr)
		{
			hwManager.freeItem(&OutputC);
		}
		if (Icp != nullptr)
		{
			hwManager.freeItem(&Icp);
		}
		if (ExtClk != nullptr)
		{
			hwManager.freeItem(&ExtClk);
		}
	}
	Owner = 0;
}

int8_t AbstractTimer1::setWGM(uint8_t value)
{
	if ((value <= 15) && (value != 13))
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			TCCR1A = ((TCCR1A & ~3) | (value & 3));
			TCCR1B = ((TCCR1B & ~(3 << 3)) | ((value & ~3) << 1));
		}
		return 0;
	}
	else
	{
		return -1;
	}
}

int8_t AbstractTimer1::setPrescalar(uint8_t potency)
{
	uint8_t prescalar;

	if (potency == 0) //1
	{
		prescalar = 1;
	}
	else if (potency == 3) //8
	{
		prescalar = 2;
	}
	else if ((potency == 6) || (potency == 8) || (potency == 10)) //64/256/1024
	{
		prescalar = (potency >> 1); //divide by 2: 6->3, 8->4, 10->5
	}
	else
	{
		return -1;
	}

	TCCR1B = ((TCCR1B & ~7) | prescalar);
	return 0;
}

void AbstractTimer1::stop()
{
	TCCR1B &= ~7;
}

void AbstractTimer1::setCounter(uint16_t value)
{
	TCNT1 = value;
}

uint16_t AbstractTimer1::getCounter()
{
	return TCNT1;
}

int8_t AbstractTimer1::setOCRx(uint16_t value, char channel)
{
	switch (channel)
	{
		case 'A':
			OCR1A = value;
			return 0;
		case 'B':
			OCR1B = value;
			return 0;
		case 'C':
			OCR1C = value;
			return 0;
		default:
			return 1;
	}
}

int8_t AbstractTimer1::forceOutputCompare(char channel)
{
	if ((TCCR1A & 3) || (TCCR1B & (3 << 3)) == (1 << 4)) //Pwm-Mode?
	{
		return -1;
	}

	switch(channel)
	{
		case 'A':
			TCCR1C |= (1 << 7);
			return 0;
		case 'B':
			TCCR1C |= (1 << 6);
			return 0;
		case 'C':
			TCCR1C |= (1 << 5);
			return 0;
		default:
			return 1;
	}
}

int8_t AbstractTimer1::setCOMx(uint8_t value, char channel)
{
	if (value > 3)
	{
		return -1;
	}

	uint8_t shift;
	AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);

	switch(channel)
	{
		case 'A':
			if (value && (OutputA == nullptr))
			{
				OutputA = hwManager.allocIoPin<'B', 5>();
				if (OutputA == nullptr)
				{
					return 2;
				}
			}
			shift = 6;
			break;

		case 'B':
			if (value && (OutputB == nullptr))
			{
				OutputB = hwManager.allocIoPin<'B', 6>();
				if (OutputB == nullptr)
				{
					return 2;
				}
			}
			shift = 4;
			break;

		case 'C':
			if (value && (OutputC == nullptr))
			{
				OutputC = hwManager.allocIoPin<'B', 7>();
				if (OutputC == nullptr)
				{
					return 2;
				}
			}
			shift = 2;
			break;

		default:
			return 1;
	}

	TCCR1A = ((TCCR1A & ~(3 << shift)) | (value << shift));
	return 0;
}

IoPin* AbstractTimer1::getOutputPin(char channel)
{
	AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);

	switch(channel)
	{
		case 'A':
			if (OutputA == nullptr)
			{
				OutputA = hwManager.allocIoPin<'B', 5>();
			}
			return OutputA;

		case 'B':
			if (OutputB == nullptr)
			{
				OutputB = hwManager.allocIoPin<'B', 6>();
			}
			return OutputB;

		case 'C':
			if (OutputC == nullptr)
			{
				OutputC = hwManager.allocIoPin<'B', 7>();
			}
			return OutputC;

		default:
			return nullptr;
	}
}

int8_t AbstractTimer1::setExtClockInput(EdgeTrigger trigger)
{
	if ((trigger == EdgeTrigger::FallingEdge) || (trigger == EdgeTrigger::RisingEdge))
	{
		TCCR1B = ((TCCR1B & ~7) | (uint8_t)(trigger + 6));
		return 0;
	}
	else
	{
		return -1;
	}
}

IoPin* AbstractTimer1::getExtClkPin()
{
	if (ExtClk == nullptr)
	{
		AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);
		ExtClk = hwManager.allocIoPin<'D', 6>();
	}
	return ExtClk;
}

void AbstractTimer1::setInputCaptureValue(uint16_t value)
{
	ICR1 = value;
}

uint16_t AbstractTimer1::getInputCaptureValue()
{
	return ICR1;
}

void AbstractTimer1::setInputCaptureTrigger(EdgeTrigger trigger)
{
	TCCR1B = ((TCCR1B & ~(1 << 6)) | ((uint8_t)(trigger) << 6));
}

void AbstractTimer1::enableInputCaptureNoiseFilter(bool value)
{
	if (value)
	{
		TCCR1B |= (1 << 7);
	}
	else
	{
		TCCR1B &= ~(1 << 7);
	}
}

IoPin* AbstractTimer1::getInputCapturePin()
{
	if (Icp == nullptr)
	{
		AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);
		Icp = hwManager.allocIoPin<'D', 4>();
	}
	return Icp;
}

void AbstractTimer1::enableOutputCompareInterrupt(bool value, char channel)
{
	if ((channel >= 'A') && (channel <= 'C'))
	{
		uint8_t shift = (channel - ('A' - 1));

		TIFR1 |= (1 << shift);
		if (value)
		{
			TIMSK1 |= (1 << shift);
		}
		else
		{
			TIMSK1 &= ~(1 << shift);
		}
	}
}

void AbstractTimer1::enableTimerOverflowInterrupt(bool value)
{
	TIFR1 |= 1;
	if (value)
	{
		TIMSK1 |= 1;
	}
	else
	{
		TIMSK1 &= ~1;
	}
}

void AbstractTimer1::enableInputCaptureInterrupt(bool value)
{
	TIFR1 |= (1 << 5);
	if (value)
	{
		TIMSK1 |= (1 << 5);
	}
	else
	{
		TIMSK1 &= ~(1 << 5);
	}
}

void AbstractTimer1::setOutputCompareISR(Listener* isr, char channel)
{
	switch (channel)
	{
		case 'A':
			#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF1A
			InterruptManager::setOCF1A(isr);
			#endif
			return;
		case 'B':
			#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF1B
			InterruptManager::setOCF1B(isr);
			#endif
			return;
		case 'C':
			#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF1C
			InterruptManager::setOCF1C(isr);
			#endif
			return;
		default:
			return;
	}
}

void AbstractTimer1::setTimerOverflowISR(Listener* isr)
{
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV1
	InterruptManager::setTOV1(isr);
	#endif
}

void AbstractTimer1::setInputCaptureISR(Listener* isr)
{
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF1
	InterruptManager::setICF1(isr);
	#endif
}
