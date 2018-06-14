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

		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV3
		Listener* Tov3Listener = nullptr;
		ISR(TIMER3_OVF_vect)
		{
			if (Tov3Listener)
			{
				Tov3Listener->trigger();
			}
		}
		#endif

		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF3A
		Listener* Ocf3AListener = nullptr;
		ISR(TIMER3_COMPA_vect)
		{
			if (Ocf3AListener)
			{
				Ocf3AListener->trigger();
			}
		}
		#endif

		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF3B
		Listener* Ocf3BListener = nullptr;
		ISR(TIMER3_COMPB_vect)
		{
			if (Ocf3BListener)
			{
				Ocf3BListener->trigger();
			}
		}
		#endif

		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF3C
		Listener* Ocf3CListener = nullptr;
		ISR(TIMER3_COMPC_vect)
		{
			if (Ocf3CListener)
			{
				Ocf3CListener->trigger();
			}
		}
		#endif

		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF3
		Listener* Icf3Listener = nullptr;
		ISR(TIMER3_CAPT_vect)
		{
			if (Icf3Listener)
			{
				Icf3Listener->trigger();
			}
		}
		#endif

		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV4
		Listener* Tov4Listener = nullptr;
		ISR(TIMER4_OVF_vect)
		{
			if (Tov4Listener)
			{
				Tov4Listener->trigger();
			}
		}
		#endif

		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF4A
		Listener* Ocf4AListener = nullptr;
		ISR(TIMER4_COMPA_vect)
		{
			if (Ocf4AListener)
			{
				Ocf4AListener->trigger();
			}
		}
		#endif

		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF4B
		Listener* Ocf4BListener = nullptr;
		ISR(TIMER4_COMPB_vect)
		{
			if (Ocf4BListener)
			{
				Ocf4BListener->trigger();
			}
		}
		#endif

		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF4C
		Listener* Ocf4CListener = nullptr;
		ISR(TIMER4_COMPC_vect)
		{
			if (Ocf4CListener)
			{
				Ocf4CListener->trigger();
			}
		}
		#endif

		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF4
		Listener* Icf4Listener = nullptr;
		ISR(TIMER4_CAPT_vect)
		{
			if (Icf4Listener)
			{
				Icf4Listener->trigger();
			}
		}
		#endif

		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV5
		Listener* Tov5Listener = nullptr;
		ISR(TIMER5_OVF_vect)
		{
			if (Tov5Listener)
			{
				Tov5Listener->trigger();
			}
		}
		#endif

		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF5A
		Listener* Ocf5AListener = nullptr;
		ISR(TIMER5_COMPA_vect)
		{
			if (Ocf5AListener)
			{
				Ocf5AListener->trigger();
			}
		}
		#endif

		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF5B
		Listener* Ocf5BListener = nullptr;
		ISR(TIMER5_COMPB_vect)
		{
			if (Ocf5BListener)
			{
				Ocf5BListener->trigger();
			}
		}
		#endif

		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF5C
		Listener* Ocf5CListener = nullptr;
		ISR(TIMER5_COMPC_vect)
		{
			if (Ocf5CListener)
			{
				Ocf5CListener->trigger();
			}
		}
		#endif

		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF5
		Listener* Icf5Listener = nullptr;
		ISR(TIMER5_CAPT_vect)
		{
			if (Icf5Listener)
			{
				Icf5Listener->trigger();
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

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV3
	void setTOV3(Listener* listener)
	{
		Tov3Listener = listener;
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF3A
	void setOCF3A(Listener* listener)
	{
		Ocf3AListener = listener;
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF3B
	void setOCF3B(Listener* listener)
	{
		Ocf3BListener = listener;
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF3C
	void setOCF3C(Listener* listener)
	{
		Ocf3CListener = listener;
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF3
	void setICF3(Listener* listener)
	{
		Icf3Listener = listener;
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV4
	void setTOV4(Listener* listener)
	{
		Tov4Listener = listener;
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF4A
	void setOCF4A(Listener* listener)
	{
		Ocf4AListener = listener;
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF4B
	void setOCF4B(Listener* listener)
	{
		Ocf4BListener = listener;
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF4C
	void setOCF4C(Listener* listener)
	{
		Ocf4CListener = listener;
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF4
	void setICF4(Listener* listener)
	{
		Icf4Listener = listener;
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV5
	void setTOV5(Listener* listener)
	{
		Tov5Listener = listener;
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF5A
	void setOCF5A(Listener* listener)
	{
		Ocf5AListener = listener;
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF5B
	void setOCF5B(Listener* listener)
	{
		Ocf5BListener = listener;
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF5C
	void setOCF5C(Listener* listener)
	{
		Ocf5CListener = listener;
	}
	#endif

	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF5
	void setICF5(Listener* listener)
	{
		Icf5Listener = listener;
	}
	#endif
}
