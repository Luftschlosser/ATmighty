/*!
 * Implementation if namespace \see InterruptManager
 */

#include "InterruptManager.h"
#include <avr/interrupt.h>
#include "ATmighty/Interfaces/Listener.h"


namespace InterruptManager
{
	namespace //anonymous namespace for "private" members
	{
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV0
		Listener* Tov0Listener = nullptr;
		ISR(TIMER0_OVF_vect)
		{
			if (Tov0Listener)
			{
				Tov0Listener->trigger();
			}
		}
		#endif

		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF0A
		Listener* Ocf0AListener = nullptr;
		ISR(TIMER0_COMPA_vect)
		{
			if (Ocf0AListener)
			{
				Ocf0AListener->trigger();
			}
		}
		#endif

		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF0B
		Listener* Ocf0BListener = nullptr;
		ISR(TIMER0_COMPB_vect)
		{
			if (Ocf0BListener)
			{
				Ocf0BListener->trigger();
			}
		}
		#endif

		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV1
		Listener* Tov1Listener = nullptr;
		ISR(TIMER1_OVF_vect)
		{
			if (Tov1Listener)
			{
				Tov1Listener->trigger();
			}
		}
		#endif

		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF1A
		Listener* Ocf1AListener = nullptr;
		ISR(TIMER1_COMPA_vect)
		{
			if (Ocf1AListener)
			{
				Ocf1AListener->trigger();
			}
		}
		#endif

		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF1B
		Listener* Ocf1BListener = nullptr;
		ISR(TIMER1_COMPB_vect)
		{
			if (Ocf1BListener)
			{
				Ocf1BListener->trigger();
			}
		}
		#endif

		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF1C
		Listener* Ocf1CListener = nullptr;
		ISR(TIMER1_COMPC_vect)
		{
			if (Ocf1CListener)
			{
				Ocf1CListener->trigger();
			}
		}
		#endif

		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF1
		Listener* Icf1Listener = nullptr;
		ISR(TIMER1_CAPT_vect)
		{
			if (Icf1Listener)
			{
				Icf1Listener->trigger();
			}
		}
		#endif

		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV2
		Listener* Tov2Listener = nullptr;
		ISR(TIMER2_OVF_vect)
		{
			if (Tov2Listener)
			{
				Tov2Listener->trigger();
			}
		}
		#endif

		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF2A
		Listener* Ocf2AListener = nullptr;
		ISR(TIMER2_COMPA_vect)
		{
			if (Ocf2AListener)
			{
				Ocf2AListener->trigger();
			}
		}
		#endif

		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF2B
		Listener* Ocf2BListener = nullptr;
		ISR(TIMER2_COMPB_vect)
		{
			if (Ocf2BListener)
			{
				Ocf2BListener->trigger();
			}
		}
		#endif
	}


	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV0
	void setTOV0(Listener* listener)
	{
		Tov0Listener = listener;
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF0A
	void setOCF0A(Listener* listener)
	{
		Ocf0AListener = listener;
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF0B
	void setOCF0B(Listener* listener)
	{
		Ocf0BListener = listener;
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV1
	void setTOV1(Listener* listener)
	{
		Tov1Listener = listener;
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF1A
	void setOCF1A(Listener* listener)
	{
		Ocf1AListener = listener;
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF1B
	void setOCF1B(Listener* listener)
	{
		Ocf1BListener = listener;
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF1C
	void setOCF1C(Listener* listener)
	{
		Ocf1CListener = listener;
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF1
	void setICF1(Listener* listener)
	{
		Icf1Listener = listener;
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV2
	void setTOV2(Listener* listener)
	{
		Tov2Listener = listener;
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF2A
	void setOCF2A(Listener* listener)
	{
		Ocf2AListener = listener;
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF2B
	void setOCF2B(Listener* listener)
	{
		Ocf2BListener = listener;
	}
	#endif
}
