/*!
 * This file implements class \see AbstractTimer0
 */


#include "AbstractTimer0.h"
#include <avr/io.h>
#include <util/atomic.h>
#include <avr/interrupt.h>
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"
#include "ATmighty/Ressources/Periphery/Abstract/AbstractHardwareManager.h"
#include "ATmighty/Utilities/LUTs/HardwareOwnerID.h"
#include "ATmighty/Interfaces/Listener.h"
#include "ATmighty/Ressources/Interrupts/InterruptManager.h"


//AbstractTimer0-class implementation

int8_t AbstractTimer0::Owner = 0;
SpecificIoPin<'B',7>* AbstractTimer0::OutputA = nullptr;
SpecificIoPin<'G',5>* AbstractTimer0::OutputB = nullptr;
SpecificIoPin<'D',7>* AbstractTimer0::ExtClk = nullptr;

int8_t AbstractTimer0::init(int8_t ownerId)
{
	if (Owner == 0)
	{
		physicalReference = PhysicalHardwareManager::Alloc<Timer0>(OwnerID::DirectAbstraction);

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

void AbstractTimer0::exit()
{
	PhysicalHardwareManager::Free(&physicalReference);
	if (OutputA || OutputB || ExtClk)
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
		if (ExtClk != nullptr)
		{
			hwManager.freeItem(&ExtClk);
		}
	}
	Owner = 0;
}

int8_t AbstractTimer0::setWGM(uint8_t value)
{
	if ((value <= 7) && (value != 6) && (value != 4))
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			TCCR0A = ((TCCR0A & ~3) | (value & 3));
			if (value > 3)
			{
				TCCR0B |= (1<<3);
			}
			else
			{
				TCCR0B &= ~(1<<3);
			}
		}
		return 0;
	}
	else
	{
		return -1;
	}
}

int8_t AbstractTimer0::setPrescalar(uint8_t potency)
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

	TCCR0B = ((TCCR0B & ~7) | prescalar);
	return 0;
}

void AbstractTimer0::stop()
{
	TCCR0B &= ~7;
}

void AbstractTimer0::setCounter(uint8_t value)
{
	TCNT0 = value;
}

uint8_t AbstractTimer0::getCounter()
{
	return TCNT0;
}

int8_t AbstractTimer0::setOCRx(uint8_t value, char channel)
{
	if (channel == 'A')
	{
		OCR0A = value;
		return 0;
	}
	else if (channel == 'B')
	{
		OCR0B = value;
		return 0;
	}
	else
	{
		return 1;
	}
}

int8_t AbstractTimer0::forceOutputCompare(char channel)
{
	if (TCCR0A & 1) //WGM0 == 1 -> PWM-Mode
	{
		return -1;
	}

	if (channel == 'A')
	{
		TCCR0B |= (1<<7);
		return 0;
	}
	else if (channel == 'B')
	{
		TCCR0B |= (1<<6);
		return 0;
	}
	else
	{
		return 1;
	}
}

int8_t AbstractTimer0::setCOMx(uint8_t value, char channel)
{
	if (value > 3)
	{
		return -1;
	}

	if (channel == 'A')
	{
		if (value)
		{
			if (OutputA == nullptr) //pin not allocated jet
			{
				AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);
				OutputA = hwManager.allocIoPin<'B', 7>();
				if (OutputA == nullptr)
				{
					return 2;
				}
			}
		}
		TCCR0A = ((TCCR0A & ~(3 << 6)) | (value << 6));
		return 0;
	}
	else if (channel == 'B')
	{
		if (value)
		{
			if (OutputB == nullptr) //pin not allocated jet
			{
				AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);
				OutputB = hwManager.allocIoPin<'G', 5>();
				if (OutputB == nullptr)
				{
					return 2;
				}
			}
		}
		TCCR0A = ((TCCR0A & ~(3 << 4)) | (value << 4));
		return 0;
	}

	return 1;
}

IoPin* AbstractTimer0::getOutputPin(char channel)
{
	if (channel == 'A')
	{
		if (OutputA == nullptr)
		{
			AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);
			OutputA = hwManager.allocIoPin<'B', 7>();
		}
		return OutputA;
	}
	if (channel == 'B')
	{
		if (OutputB == nullptr)
		{
			AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);
			OutputB = hwManager.allocIoPin<'G', 5>();
		}
		return OutputB;
	}
	return nullptr;
}

int8_t AbstractTimer0::setExtClockInput(ExtClkTrigger trigger)
{
	if ((trigger == ExtClkTrigger::FallingEdge) || (trigger == ExtClkTrigger::RisingEdge))
	{
		TCCR0B = ((TCCR0B & ~7) | (uint8_t)trigger);
		return 0;
	}
	else
	{
		return -1;
	}
}

IoPin* AbstractTimer0::getExtClkPin()
{
	if (ExtClk == nullptr)
	{
		AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);
		ExtClk = hwManager.allocIoPin<'D', 7>();
	}
	return ExtClk;
}

void AbstractTimer0::enableOutputCompareInterrupt(bool value, char channel)
{
	if (channel == 'A')
	{
		TIFR0 |= (1 << 1);
		if (value)
		{
			TIMSK0 |= (1 << 1);
		}
		else
		{
			TIMSK0 &= ~(1 << 1);
		}
	}
	else if (channel == 'B')
	{
		TIFR0 |= (1 << 2);
		if (value)
		{
			TIMSK0 |= (1 << 2);
		}
		else
		{
			TIMSK0 &= ~(1 << 2);
		}
	}
}

void AbstractTimer0::enableTimerOverflowInterrupt(bool value)
{
	TIFR0 |= 1;
	if (value)
	{
		TIMSK0 |= 1;
	}
	else
	{
		TIMSK0 &= ~1;
	}
}

void AbstractTimer0::setOutputCompareISR(Listener* isr, char channel)
{
	if (channel == 'A')
	{
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF0A
		InterruptManager::setOCF0A(isr);
		#endif
	}
	else if (channel == 'B')
	{
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF0B
		InterruptManager::setOCF0B(isr);
		#endif
	}
}

void AbstractTimer0::setOutputCompareISR(void (*isr)(), char channel)
{
	if (channel == 'A')
	{
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF0A
		InterruptManager::setOCF0A(isr);
		#endif
	}
	else if (channel == 'B')
	{
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF0B
		InterruptManager::setOCF0B(isr);
		#endif
	}
}

void AbstractTimer0::setTimerOverflowISR(Listener* isr)
{
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV0
	InterruptManager::setTOV0(isr);
	#endif
}

void AbstractTimer0::setTimerOverflowISR(void (*isr)())
{
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV0
	InterruptManager::setTOV0(isr);
	#endif
}
