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
#include "Config/InterruptConfig.h"


//ISR implementations
namespace Timer0ISR
{
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV0
	Listener* TovListener = nullptr;

	ISR(TIMER0_OVF_vect)
	{
		if (TovListener)
		{
			TovListener->trigger();
		}
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF0A
	Listener* OcfAListener = nullptr;

	ISR(TIMER0_COMPA_vect)
	{
		if (OcfAListener)
		{
			OcfAListener->trigger();
		}
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF0A
	Listener* OcfBListener = nullptr;

	ISR(TIMER0_COMPB_vect)
	{
		if (OcfBListener)
		{
			OcfBListener->trigger();
		}
	}
	#endif
}


//AbstractTimer0-class implementation

int8_t AbstractTimer0::Owner = 0;

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
	if (outputA || outputB || extClk)
	{
		AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);
		if (outputA != nullptr)
		{
			hwManager.freeItem(&outputA);
		}
		if (outputB != nullptr)
		{
			hwManager.freeItem(&outputB);
		}
		if (extClk != nullptr)
		{
			hwManager.freeItem(&extClk);
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
			if (outputA == nullptr) //pin not allocated jet
			{
				AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);
				outputA = hwManager.allocIoPin<'B', 7>();
				if (outputA == nullptr)
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
			if (outputB == nullptr) //pin not allocated jet
			{
				AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);
				outputB = hwManager.allocIoPin<'G', 5>();
				if (outputB == nullptr)
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
		if (outputA == nullptr)
		{
			AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);
			outputA = hwManager.allocIoPin<'B', 7>();
		}
		return outputA;
	}
	if (channel == 'B')
	{
		if (outputB == nullptr)
		{
			AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);
			outputB = hwManager.allocIoPin<'G', 5>();
		}
		return outputB;
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
	if (extClk == nullptr)
	{
		AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);
		extClk = hwManager.allocIoPin<'D', 7>();
	}
	return extClk;
}

void AbstractTimer0::enableOutputCompareInterrupt(bool value, char channel)
{
	if (channel == 'A')
	{
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
		Timer0ISR::OcfAListener = isr;
		#endif
	}
	else if (channel == 'B')
	{
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF0B
		Timer0ISR::OcfBListener = isr;
		#endif
	}
}

void AbstractTimer0::setTimerOverflowISR(Listener* isr)
{
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV0
	Timer0ISR::TovListener = isr;
	#endif
}
