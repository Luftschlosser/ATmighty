/*!
 * This file implements class \see AbstractTimer5
 */


#include "AbstractTimer5.h"
#include <avr/io.h>
#include <util/atomic.h>
#include <avr/interrupt.h>
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"
#include "ATmighty/Ressources/Periphery/Abstract/AbstractHardwareManager.h"
#include "ATmighty/Utilities/LUTs/HardwareOwnerID.h"
#include "ATmighty/Interfaces/Listener.h"
#include "ATmighty/Ressources/Interrupts/InterruptManager.h"


//AbstractTimer5-class implementation

int8_t AbstractTimer5::Owner = 0;
SpecificIoPin<'L',3>* AbstractTimer5::OutputA = nullptr;
SpecificIoPin<'L',4>* AbstractTimer5::OutputB = nullptr;
SpecificIoPin<'L',5>* AbstractTimer5::OutputC = nullptr;
SpecificIoPin<'L',2>* AbstractTimer5::ExtClk = nullptr;
SpecificIoPin<'L',1>* AbstractTimer5::Icp = nullptr;

int8_t AbstractTimer5::init(int8_t ownerId)
{
	if (Owner == 0)
	{
		physicalReference = PhysicalHardwareManager::Alloc<Timer5>(OwnerID::DirectAbstraction);

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

void AbstractTimer5::exit()
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

int8_t AbstractTimer5::setWGM(uint8_t value)
{
	if ((value <= 15) && (value != 13))
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			TCCR5A = ((TCCR5A & ~3) | (value & 3));
			TCCR5B = ((TCCR5B & ~(3 << 3)) | ((value & ~3) << 1));
		}
		return 0;
	}
	else
	{
		return -1;
	}
}

int8_t AbstractTimer5::setPrescalar(uint8_t potency)
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

	TCCR5B = ((TCCR5B & ~7) | prescalar);
	return 0;
}

void AbstractTimer5::stop()
{
	TCCR5B &= ~7;
}

void AbstractTimer5::setCounter(uint16_t value)
{
	TCNT5 = value;
}

uint16_t AbstractTimer5::getCounter()
{
	return TCNT5;
}

int8_t AbstractTimer5::setOCRx(uint16_t value, char channel)
{
	switch (channel)
	{
		case 'A':
			OCR5A = value;
			return 0;
		case 'B':
			OCR5B = value;
			return 0;
		case 'C':
			OCR5C = value;
			return 0;
		default:
			return 1;
	}
}

int8_t AbstractTimer5::forceOutputCompare(char channel)
{
	if ((TCCR5A & 3) || (TCCR5B & (3 << 3)) == (1 << 4)) //Pwm-Mode?
	{
		return -1;
	}

	switch(channel)
	{
		case 'A':
			TCCR5C |= (1 << 7);
			return 0;
		case 'B':
			TCCR5C |= (1 << 6);
			return 0;
		case 'C':
			TCCR5C |= (1 << 5);
			return 0;
		default:
			return 1;
	}
}

int8_t AbstractTimer5::setCOMx(uint8_t value, char channel)
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
				OutputA = hwManager.allocIoPin<'L', 3>();
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
				OutputB = hwManager.allocIoPin<'L', 4>();
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
				OutputC = hwManager.allocIoPin<'L', 5>();
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

	TCCR5A = ((TCCR5A & ~(3 << shift)) | (value << shift));
	return 0;
}

IoPin* AbstractTimer5::getOutputPin(char channel)
{
	AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);

	switch(channel)
	{
		case 'A':
			if (OutputA == nullptr)
			{
				OutputA = hwManager.allocIoPin<'L', 3>();
			}
			return OutputA;

		case 'B':
			if (OutputB == nullptr)
			{
				OutputB = hwManager.allocIoPin<'L', 4>();
			}
			return OutputB;

		case 'C':
			if (OutputC == nullptr)
			{
				OutputC = hwManager.allocIoPin<'L', 5>();
			}
			return OutputC;

		default:
			return nullptr;
	}
}

int8_t AbstractTimer5::setExtClockInput(EdgeTrigger trigger)
{
	if ((trigger == EdgeTrigger::FallingEdge) || (trigger == EdgeTrigger::RisingEdge))
	{
		TCCR5B = ((TCCR5B & ~7) | (uint8_t)(trigger + 6));
		return 0;
	}
	else
	{
		return -1;
	}
}

IoPin* AbstractTimer5::getExtClkPin()
{
	if (ExtClk == nullptr)
	{
		AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);
		ExtClk = hwManager.allocIoPin<'L', 2>();
	}
	return ExtClk;
}

void AbstractTimer5::setInputCaptureValue(uint16_t value)
{
	ICR5 = value;
}

uint16_t AbstractTimer5::getInputCaptureValue()
{
	return ICR5;
}

void AbstractTimer5::setInputCaptureTrigger(EdgeTrigger trigger)
{
	TCCR5B = ((TCCR5B & ~(1 << 6)) | ((uint8_t)(trigger) << 6));
}

void AbstractTimer5::enableInputCaptureNoiseFilter(bool value)
{
	if (value)
	{
		TCCR5B |= (1 << 7);
	}
	else
	{
		TCCR5B &= ~(1 << 7);
	}
}

IoPin* AbstractTimer5::getInputCapturePin()
{
	if (Icp == nullptr)
	{
		AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);
		Icp = hwManager.allocIoPin<'L', 1>();
	}
	return Icp;
}

void AbstractTimer5::enableOutputCompareInterrupt(bool value, char channel)
{
	if ((channel >= 'A') && (channel <= 'C'))
	{
		uint8_t shift = (channel - ('A' - 1));

		TIFR5 |= (1 << shift);
		if (value)
		{
			TIMSK5 |= (1 << shift);
		}
		else
		{
			TIMSK5 &= ~(1 << shift);
		}
	}
}

void AbstractTimer5::enableTimerOverflowInterrupt(bool value)
{
	TIFR5 |= 1;
	if (value)
	{
		TIMSK5 |= 1;
	}
	else
	{
		TIMSK5 &= ~1;
	}
}

void AbstractTimer5::enableInputCaptureInterrupt(bool value)
{
	TIFR5 |= (1 << 5);
	if (value)
	{
		TIMSK5 |= (1 << 5);
	}
	else
	{
		TIMSK5 &= ~(1 << 5);
	}
}

void AbstractTimer5::setOutputCompareISR(Listener* isr, char channel)
{
	switch (channel)
	{
		case 'A':
			#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF5A
			InterruptManager::setOCF5A(isr);
			#endif
			return;
		case 'B':
			#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF5B
			InterruptManager::setOCF5B(isr);
			#endif
			return;
		case 'C':
			#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF5C
			InterruptManager::setOCF5C(isr);
			#endif
			return;
		default:
			return;
	}
}

void AbstractTimer5::setTimerOverflowISR(Listener* isr)
{
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV5
	InterruptManager::setTOV5(isr);
	#endif
}

void AbstractTimer5::setInputCaptureISR(Listener* isr)
{
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF5
	InterruptManager::setICF5(isr);
	#endif
}
