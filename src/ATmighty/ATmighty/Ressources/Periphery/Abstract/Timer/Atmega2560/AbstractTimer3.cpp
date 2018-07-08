/*!
 * This file implements class \see AbstractTimer3
 */


#include "AbstractTimer3.h"
#include <avr/io.h>
#include <util/atomic.h>
#include <avr/interrupt.h>
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"
#include "ATmighty/Ressources/Periphery/Abstract/AbstractHardwareManager.h"
#include "ATmighty/Utilities/LUTs/HardwareOwnerID.h"
#include "ATmighty/Interfaces/Listener.h"
#include "ATmighty/Ressources/Interrupts/InterruptManager.h"


//AbstractTimer3-class implementation

int8_t AbstractTimer3::Owner = 0;
SpecificIoPin<'E',3>* AbstractTimer3::OutputA = nullptr;
SpecificIoPin<'E',4>* AbstractTimer3::OutputB = nullptr;
SpecificIoPin<'E',5>* AbstractTimer3::OutputC = nullptr;
SpecificIoPin<'E',6>* AbstractTimer3::ExtClk = nullptr;
SpecificIoPin<'E',7>* AbstractTimer3::Icp = nullptr;

int8_t AbstractTimer3::init(int8_t ownerId)
{
	if (Owner == 0)
	{
		physicalReference = PhysicalHardwareManager::Alloc<Timer3>(OwnerID::DirectAbstraction);

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

void AbstractTimer3::exit()
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

int8_t AbstractTimer3::setWGM(uint8_t value)
{
	if ((value <= 15) && (value != 13))
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			TCCR3A = ((TCCR3A & ~3) | (value & 3));
			TCCR3B = ((TCCR3B & ~(3 << 3)) | ((value & ~3) << 1));
		}
		return 0;
	}
	else
	{
		return -1;
	}
}

int8_t AbstractTimer3::setPrescalar(uint8_t potency)
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

	TCCR3B = ((TCCR3B & ~7) | prescalar);
	return 0;
}

void AbstractTimer3::stop()
{
	TCCR3B &= ~7;
}

void AbstractTimer3::setCounter(uint16_t value)
{
	TCNT3 = value;
}

uint16_t AbstractTimer3::getCounter()
{
	return TCNT3;
}

int8_t AbstractTimer3::setOCRx(uint16_t value, char channel)
{
	switch (channel)
	{
		case 'A':
			OCR3A = value;
			return 0;
		case 'B':
			OCR3B = value;
			return 0;
		case 'C':
			OCR3C = value;
			return 0;
		default:
			return 1;
	}
}

int8_t AbstractTimer3::forceOutputCompare(char channel)
{
	if ((TCCR3A & 3) || (TCCR3B & (3 << 3)) == (1 << 4)) //Pwm-Mode?
	{
		return -1;
	}

	switch(channel)
	{
		case 'A':
			TCCR3C |= (1 << 7);
			return 0;
		case 'B':
			TCCR3C |= (1 << 6);
			return 0;
		case 'C':
			TCCR3C |= (1 << 5);
			return 0;
		default:
			return 1;
	}
}

int8_t AbstractTimer3::setCOMx(uint8_t value, char channel)
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
				OutputA = hwManager.allocIoPin<'E', 3>();
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
				OutputB = hwManager.allocIoPin<'E', 4>();
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
				OutputC = hwManager.allocIoPin<'E', 5>();
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

	TCCR3A = ((TCCR3A & ~(3 << shift)) | (value << shift));
	return 0;
}

IoPin* AbstractTimer3::getOutputPin(char channel)
{
	AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);

	switch(channel)
	{
		case 'A':
			if (OutputA == nullptr)
			{
				OutputA = hwManager.allocIoPin<'E', 3>();
			}
			return OutputA;

		case 'B':
			if (OutputB == nullptr)
			{
				OutputB = hwManager.allocIoPin<'E', 4>();
			}
			return OutputB;

		case 'C':
			if (OutputC == nullptr)
			{
				OutputC = hwManager.allocIoPin<'E', 5>();
			}
			return OutputC;

		default:
			return nullptr;
	}
}

int8_t AbstractTimer3::setExtClockInput(EdgeTrigger trigger)
{
	if ((trigger == EdgeTrigger::FallingEdge) || (trigger == EdgeTrigger::RisingEdge))
	{
		TCCR3B = ((TCCR3B & ~7) | (uint8_t)(trigger + 6));
		return 0;
	}
	else
	{
		return -1;
	}
}

IoPin* AbstractTimer3::getExtClkPin()
{
	if (ExtClk == nullptr)
	{
		AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);
		ExtClk = hwManager.allocIoPin<'E', 6>();
	}
	return ExtClk;
}

void AbstractTimer3::setInputCaptureValue(uint16_t value)
{
	ICR3 = value;
}

uint16_t AbstractTimer3::getInputCaptureValue()
{
	return ICR3;
}

void AbstractTimer3::setInputCaptureTrigger(EdgeTrigger trigger)
{
	TCCR3B = ((TCCR3B & ~(1 << 6)) | ((uint8_t)(trigger) << 6));
}

void AbstractTimer3::enableInputCaptureNoiseFilter(bool value)
{
	if (value)
	{
		TCCR3B |= (1 << 7);
	}
	else
	{
		TCCR3B &= ~(1 << 7);
	}
}

IoPin* AbstractTimer3::getInputCapturePin()
{
	if (Icp == nullptr)
	{
		AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);
		Icp = hwManager.allocIoPin<'E', 7>();
	}
	return Icp;
}

void AbstractTimer3::enableOutputCompareInterrupt(bool value, char channel)
{
	if ((channel >= 'A') && (channel <= 'C'))
	{
		uint8_t shift = (channel - ('A' - 1));

		TIFR3 |= (1 << shift);
		if (value)
		{
			TIMSK3 |= (1 << shift);
		}
		else
		{
			TIMSK3 &= ~(1 << shift);
		}
	}
}

void AbstractTimer3::enableTimerOverflowInterrupt(bool value)
{
	TIFR3 |= 1;
	if (value)
	{
		TIMSK3 |= 1;
	}
	else
	{
		TIMSK3 &= ~1;
	}
}

void AbstractTimer3::enableInputCaptureInterrupt(bool value)
{
	TIFR3 |= (1 << 5);
	if (value)
	{
		TIMSK3 |= (1 << 5);
	}
	else
	{
		TIMSK3 &= ~(1 << 5);
	}
}

void AbstractTimer3::setOutputCompareISR(Listener* isr, char channel)
{
	switch (channel)
	{
		case 'A':
			#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF3A
			InterruptManager::setOCF3A(isr);
			#endif
			return;
		case 'B':
			#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF3B
			InterruptManager::setOCF3B(isr);
			#endif
			return;
		case 'C':
			#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF3C
			InterruptManager::setOCF3C(isr);
			#endif
			return;
		default:
			return;
	}
}

void AbstractTimer3::setOutputCompareISR(void (*isr)(), char channel)
{
	switch (channel)
	{
		case 'A':
			#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF3A
			InterruptManager::setOCF3A(isr);
			#endif
			return;
		case 'B':
			#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF3B
			InterruptManager::setOCF3B(isr);
			#endif
			return;
		case 'C':
			#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF3C
			InterruptManager::setOCF3C(isr);
			#endif
			return;
		default:
			return;
	}
}

void AbstractTimer3::setTimerOverflowISR(Listener* isr)
{
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV3
	InterruptManager::setTOV3(isr);
	#endif
}

void AbstractTimer3::setTimerOverflowISR(void (*isr)())
{
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV3
	InterruptManager::setTOV3(isr);
	#endif
}

void AbstractTimer3::setInputCaptureISR(Listener* isr)
{
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF3
	InterruptManager::setICF3(isr);
	#endif
}

void AbstractTimer3::setInputCaptureISR(void (*isr)())
{
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF3
	InterruptManager::setICF3(isr);
	#endif
}
