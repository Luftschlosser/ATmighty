/*!
 * This file implements class \see AbstractTimer4
 */


#include "AbstractTimer4.h"
#include <avr/io.h>
#include <util/atomic.h>
#include <avr/interrupt.h>
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"
#include "ATmighty/Ressources/Periphery/Abstract/AbstractHardwareManager.h"
#include "ATmighty/Utilities/LUTs/HardwareOwnerID.h"
#include "ATmighty/Interfaces/Listener.h"
#include "ATmighty/Ressources/Interrupts/InterruptManager.h"


//AbstractTimer4-class implementation

int8_t AbstractTimer4::Owner = 0;
SpecificIoPin<'H',3>* AbstractTimer4::OutputA = nullptr;
SpecificIoPin<'H',4>* AbstractTimer4::OutputB = nullptr;
SpecificIoPin<'H',5>* AbstractTimer4::OutputC = nullptr;
SpecificIoPin<'H',7>* AbstractTimer4::ExtClk = nullptr;
SpecificIoPin<'L',0>* AbstractTimer4::Icp = nullptr;

int8_t AbstractTimer4::init(int8_t ownerId)
{
	if (Owner == 0)
	{
		physicalReference = PhysicalHardwareManager::Alloc<Timer4>(OwnerID::DirectAbstraction);

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

void AbstractTimer4::exit()
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

int8_t AbstractTimer4::setWGM(uint8_t value)
{
	if ((value <= 15) && (value != 13))
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			TCCR4A = ((TCCR4A & ~3) | (value & 3));
			TCCR4B = ((TCCR4B & ~(3 << 3)) | ((value & ~3) << 1));
		}
		return 0;
	}
	else
	{
		return -1;
	}
}

int8_t AbstractTimer4::setPrescalar(uint8_t potency)
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

	TCCR4B = ((TCCR4B & ~7) | prescalar);
	return 0;
}

void AbstractTimer4::stop()
{
	TCCR4B &= ~7;
}

void AbstractTimer4::setCounter(uint16_t value)
{
	TCNT4 = value;
}

uint16_t AbstractTimer4::getCounter()
{
	return TCNT4;
}

int8_t AbstractTimer4::setOCRx(uint16_t value, char channel)
{
	switch (channel)
	{
		case 'A':
			OCR4A = value;
			return 0;
		case 'B':
			OCR4B = value;
			return 0;
		case 'C':
			OCR4C = value;
			return 0;
		default:
			return 1;
	}
}

int8_t AbstractTimer4::forceOutputCompare(char channel)
{
	if ((TCCR4A & 3) || (TCCR4B & (3 << 3)) == (1 << 4)) //Pwm-Mode?
	{
		return -1;
	}

	switch(channel)
	{
		case 'A':
			TCCR4C |= (1 << 7);
			return 0;
		case 'B':
			TCCR4C |= (1 << 6);
			return 0;
		case 'C':
			TCCR4C |= (1 << 5);
			return 0;
		default:
			return 1;
	}
}

int8_t AbstractTimer4::setCOMx(uint8_t value, char channel)
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
				OutputA = hwManager.allocIoPin<'H', 3>();
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
				OutputB = hwManager.allocIoPin<'H', 4>();
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
				OutputC = hwManager.allocIoPin<'H', 5>();
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

	TCCR4A = ((TCCR4A & ~(3 << shift)) | (value << shift));
	return 0;
}

IoPin* AbstractTimer4::getOutputPin(char channel)
{
	AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);

	switch(channel)
	{
		case 'A':
			if (OutputA == nullptr)
			{
				OutputA = hwManager.allocIoPin<'H', 3>();
			}
			return OutputA;

		case 'B':
			if (OutputB == nullptr)
			{
				OutputB = hwManager.allocIoPin<'H', 4>();
			}
			return OutputB;

		case 'C':
			if (OutputC == nullptr)
			{
				OutputC = hwManager.allocIoPin<'H', 5>();
			}
			return OutputC;

		default:
			return nullptr;
	}
}

int8_t AbstractTimer4::setExtClockInput(EdgeTrigger trigger)
{
	if ((trigger == EdgeTrigger::FallingEdge) || (trigger == EdgeTrigger::RisingEdge))
	{
		TCCR4B = ((TCCR4B & ~7) | (uint8_t)(trigger + 6));
		return 0;
	}
	else
	{
		return -1;
	}
}

IoPin* AbstractTimer4::getExtClkPin()
{
	if (ExtClk == nullptr)
	{
		AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);
		ExtClk = hwManager.allocIoPin<'H', 7>();
	}
	return ExtClk;
}

void AbstractTimer4::setInputCaptureValue(uint16_t value)
{
	ICR4 = value;
}

uint16_t AbstractTimer4::getInputCaptureValue()
{
	return ICR4;
}

void AbstractTimer4::setInputCaptureTrigger(EdgeTrigger trigger)
{
	TCCR4B = ((TCCR4B & ~(1 << 6)) | ((uint8_t)(trigger) << 6));
}

void AbstractTimer4::enableInputCaptureNoiseFilter(bool value)
{
	if (value)
	{
		TCCR4B |= (1 << 7);
	}
	else
	{
		TCCR4B &= ~(1 << 7);
	}
}

IoPin* AbstractTimer4::getInputCapturePin()
{
	if (Icp == nullptr)
	{
		AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);
		Icp = hwManager.allocIoPin<'L', 0>();
	}
	return Icp;
}

void AbstractTimer4::enableOutputCompareInterrupt(bool value, char channel)
{
	if ((channel >= 'A') && (channel <= 'C'))
	{
		uint8_t shift = (channel - ('A' - 1));

		TIFR4 |= (1 << shift);
		if (value)
		{
			TIMSK4 |= (1 << shift);
		}
		else
		{
			TIMSK4 &= ~(1 << shift);
		}
	}
}

void AbstractTimer4::enableTimerOverflowInterrupt(bool value)
{
	TIFR4 |= 1;
	if (value)
	{
		TIMSK4 |= 1;
	}
	else
	{
		TIMSK4 &= ~1;
	}
}

void AbstractTimer4::enableInputCaptureInterrupt(bool value)
{
	TIFR4 |= (1 << 5);
	if (value)
	{
		TIMSK4 |= (1 << 5);
	}
	else
	{
		TIMSK4 &= ~(1 << 5);
	}
}

void AbstractTimer4::setOutputCompareISR(Listener* isr, char channel)
{
	switch (channel)
	{
		case 'A':
			#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF4A
			InterruptManager::setOCF4A(isr);
			#endif
			return;
		case 'B':
			#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF4B
			InterruptManager::setOCF4B(isr);
			#endif
			return;
		case 'C':
			#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF4C
			InterruptManager::setOCF4C(isr);
			#endif
			return;
		default:
			return;
	}
}

void AbstractTimer4::setOutputCompareISR(void (*isr)(), char channel)
{
	switch (channel)
	{
		case 'A':
			#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF4A
			InterruptManager::setOCF4A(isr);
			#endif
			return;
		case 'B':
			#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF4B
			InterruptManager::setOCF4B(isr);
			#endif
			return;
		case 'C':
			#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF4C
			InterruptManager::setOCF4C(isr);
			#endif
			return;
		default:
			return;
	}
}

void AbstractTimer4::setTimerOverflowISR(Listener* isr)
{
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV4
	InterruptManager::setTOV4(isr);
	#endif
}

void AbstractTimer4::setTimerOverflowISR(void (*isr)())
{
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV4
	InterruptManager::setTOV4(isr);
	#endif
}

void AbstractTimer4::setInputCaptureISR(Listener* isr)
{
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF4
	InterruptManager::setICF4(isr);
	#endif
}

void AbstractTimer4::setInputCaptureISR(void (*isr)())
{
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF4
	InterruptManager::setICF4(isr);
	#endif
}
