/*!
 * This file implements class \see AbstractTimer2
 */


#include "AbstractTimer2.h"
#include <avr/io.h>
#include <util/atomic.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"
#include "ATmighty/Ressources/Periphery/Abstract/AbstractHardwareManager.h"
#include "ATmighty/Utilities/LUTs/HardwareOwnerID.h"
#include "ATmighty/Interfaces/Listener.h"
#include "Config/InterruptConfig.h"
#include "ATmighty/Utilities/Logs/MessageLog.h"
#include "ATmighty/Utilities/LUTs/MessageLogPhrases.h"


//ISR implementations
namespace Timer2ISR
{
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV2
	Listener* TovListener = nullptr;

	ISR(TIMER2_OVF_vect)
	{
		if (TovListener)
		{
			TovListener->trigger();
		}
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF2A
	Listener* OcfAListener = nullptr;

	ISR(TIMER2_COMPA_vect)
	{
		if (OcfAListener)
		{
			OcfAListener->trigger();
		}
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF2A
	Listener* OcfBListener = nullptr;

	ISR(TIMER2_COMPB_vect)
	{
		if (OcfBListener)
		{
			OcfBListener->trigger();
		}
	}
	#endif
}


//AbstractTimer2-class implementation

int8_t AbstractTimer2::Owner = 0;
SpecificIoPin<'B',4>* AbstractTimer2::OutputA = nullptr;
SpecificIoPin<'H',6>* AbstractTimer2::OutputB = nullptr;
SpecificIoPin<'G',4>* AbstractTimer2::Tosc1 = nullptr;
SpecificIoPin<'G',3>* AbstractTimer2::Tosc2 = nullptr;

int8_t AbstractTimer2::init(int8_t ownerId)
{
	if (Owner == 0)
	{
		physicalReference = PhysicalHardwareManager::Alloc<Timer2>(OwnerID::DirectAbstraction);

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

void AbstractTimer2::exit()
{
	PhysicalHardwareManager::Free(&physicalReference);
	if (OutputA || OutputB || Tosc1 || Tosc2)
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
		if (Tosc1 != nullptr)
		{
			hwManager.freeItem(&Tosc1);
		}
		if (Tosc2 != nullptr)
		{
			hwManager.freeItem(&Tosc2);
		}
	}
	Owner = 0;
}

void AbstractTimer2::signalUpdateBusyError()
{
	#if ATMIGHTY_MESSAGELOG_ENABLE
	static const char msg[] PROGMEM = ": Register write while update-busy-flag is set!";

	MessageLog<>::DefaultInstance().log<LogLevel::Warning>(true,
			MessageLogPhrases::Phrase_AbstractHw,
			getHardwareStringRepresentation(),
			'[', getCharCode(), '[',
			msg);
	#endif
}

int8_t AbstractTimer2::setWGM(uint8_t value)
{
	if ((value <= 7) && (value != 6) && (value != 4))
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			if (ASSR & 3) //Update-busy flags of TCCR2A or TCCR2B are set while in asynchronous mode?
			{
				signalUpdateBusyError();
			}

			TCCR2A = ((TCCR2A & ~3) | (value & 3));
			if (value > 3)
			{
				TCCR2B |= (1<<3);
			}
			else
			{
				TCCR2B &= ~(1<<3);
			}
		}
		return 0;
	}
	else
	{
		return -1;
	}
}

int8_t AbstractTimer2::setPrescalar(uint8_t potency)
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
	else if (potency == 5) //32
	{
		prescalar = 3;
	}
	else if (potency == 6) //64
	{
		prescalar = 4;
	}
	else if (potency == 7) //128
	{
		prescalar = 5;
	}
	else if (potency == 8) //256
	{
		prescalar = 6;
	}
	else if (potency == 10) //1024
	{
		prescalar = 7;
	}
	else
	{
		return -1;
	}

	if (ASSR & 1) //Update-busy flag of TCCR2B is set while in asynchronous mode?
	{
		signalUpdateBusyError();
	}

	TCCR2B = ((TCCR2B & ~7) | prescalar);
	return 0;
}

void AbstractTimer2::stop()
{
	if (ASSR & 1) //Update-busy flag of TCCR2B is set while in asynchronous mode?
	{
		signalUpdateBusyError();
	}

	TCCR2B &= ~7;
}

void AbstractTimer2::setCounter(uint8_t value)
{
	if (ASSR & (1 << 4)) //Update-busy flag of TCNT2 is set while in asynchronous mode?
	{
		signalUpdateBusyError();
	}

	TCNT2 = value;
}

uint8_t AbstractTimer2::getCounter()
{
	return TCNT2;
}

int8_t AbstractTimer2::setOCRx(uint8_t value, char channel)
{
	if (channel == 'A')
	{
		if (ASSR & (1 << 3)) //Update-busy flag of OCR2A is set while in asynchronous mode?
		{
			signalUpdateBusyError();
		}

		OCR2A = value;
		return 0;
	}
	else if (channel == 'B')
	{
		if (ASSR & (1 << 2)) //Update-busy flag of OCR2B is set while in asynchronous mode?
		{
			signalUpdateBusyError();
		}

		OCR2B = value;
		return 0;
	}
	else
	{
		return 1;
	}
}

int8_t AbstractTimer2::forceOutputCompare(char channel)
{
	if (TCCR2A & 1) //WGM0 == 1 -> PWM-Mode
	{
		return -1;
	}

	if (channel == 'A')
	{
		if (ASSR & 1) //Update-busy flag of TCCR2B is set while in asynchronous mode?
		{
			signalUpdateBusyError();
		}

		TCCR2B |= (1<<7);
		return 0;
	}
	else if (channel == 'B')
	{
		if (ASSR & 1) //Update-busy flag of TCCR2B is set while in asynchronous mode?
		{
			signalUpdateBusyError();
		}

		TCCR2B |= (1<<6);
		return 0;
	}
	else
	{
		return 1;
	}
}

int8_t AbstractTimer2::setCOMx(uint8_t value, char channel)
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
				OutputA = hwManager.allocIoPin<'B', 4>();
				if (OutputA == nullptr)
				{
					return 2;
				}
			}
		}

		if (ASSR & (1 << 1)) //Update-busy flag of TCCR2A is set while in asynchronous mode?
		{
			signalUpdateBusyError();
		}

		TCCR2A = ((TCCR2A & ~(3 << 6)) | (value << 6));
		return 0;
	}
	else if (channel == 'B')
	{
		if (value)
		{
			if (OutputB == nullptr) //pin not allocated jet
			{
				AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);
				OutputB = hwManager.allocIoPin<'H', 6>();
				if (OutputB == nullptr)
				{
					return 2;
				}
			}
		}
		if (ASSR & (1 << 1)) //Update-busy flag of TCCR2A is set while in asynchronous mode?
		{
			signalUpdateBusyError();
		}

		TCCR2A = ((TCCR2A & ~(3 << 4)) | (value << 4));
		return 0;
	}

	return 1;
}

IoPin* AbstractTimer2::getOutputPin(char channel)
{
	if (channel == 'A')
	{
		if (OutputA == nullptr)
		{
			AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);
			OutputA = hwManager.allocIoPin<'B', 4>();
		}
		return OutputA;
	}
	if (channel == 'B')
	{
		if (OutputB == nullptr)
		{
			AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);
			OutputB = hwManager.allocIoPin<'H', 6>();
		}
		return OutputB;
	}
	return nullptr;
}

IoPin* AbstractTimer2::getExtClkPin()
{
	if (Tosc1 == nullptr)
	{
		AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);
		Tosc1 = hwManager.allocIoPin<'G', 4>();
	}
	return Tosc1;
}

void AbstractTimer2::enableOutputCompareInterrupt(bool value, char channel)
{
	if (channel == 'A')
	{
		if (value)
		{
			TIMSK2 |= (1 << 1);
		}
		else
		{
			TIMSK2 &= ~(1 << 1);
		}
	}
	else if (channel == 'B')
	{
		if (value)
		{
			TIMSK2 |= (1 << 2);
		}
		else
		{
			TIMSK2 &= ~(1 << 2);
		}
	}
}

void AbstractTimer2::enableTimerOverflowInterrupt(bool value)
{
	if (value)
	{
		TIMSK2 |= 1;
	}
	else
	{
		TIMSK2 &= ~1;
	}
}

void AbstractTimer2::setOutputCompareISR(Listener* isr, char channel)
{
	if (channel == 'A')
	{
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF2A
		Timer2ISR::OcfAListener = isr;
		#endif
	}
	else if (channel == 'B')
	{
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF2B
		Timer2ISR::OcfBListener = isr;
		#endif
	}
}

void AbstractTimer2::setTimerOverflowISR(Listener* isr)
{
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV2
	Timer2ISR::TovListener = isr;
	#endif
}

uint32_t AbstractTimer2::getBaseFrequency()
{
	return (ASSR ? 0 : F_CPU);
}

int8_t AbstractTimer2::setAsyncMode(AsyncMode mode)
{
	if ((mode <= 3) && (mode != 2))
	{
		if ((Tosc1 == nullptr) || (Tosc2 == nullptr))
		{
			AbstractHardwareManager hwManager(OwnerID::AbstractionDependency);

			if (Tosc1 == nullptr)
			{
				Tosc1 = hwManager.allocIoPin<'G', 4>();
			}
			if (Tosc2 == nullptr)
			{
				Tosc2 = hwManager.allocIoPin<'G', 3>();
			}
		}

		if ((Tosc1 != nullptr) && (Tosc2 != nullptr))
		{
			ASSR = (mode << 5);
			return 0;
		}
		else //IoPins could not be allocated
		{
			return 2;
		}
	}
	else //invalid mode
	{
		return -1;
	}
}
