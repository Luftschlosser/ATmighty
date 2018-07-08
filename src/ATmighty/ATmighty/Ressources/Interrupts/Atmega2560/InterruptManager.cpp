/*!
 * Implementation if namespace \see InterruptManager
 */

#include "InterruptManager.h"
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "ATmighty/Interfaces/Listener.h"


namespace InterruptManager
{
	namespace //anonymous namespace for "private" members
	{
		//typedef for universal pointer
		typedef union isr
		{
			Listener* listener;	//Listener-object to trigger on interrupt
			void (*callback)();	//Functionpointer to call back on interrupt
		} isr_t;
		
		
		//define bitmasks to specify isr-type (listener = 1 / callback = 0)
		//uint8_t isr_type_02_09 = 0;
		uint8_t isr_type_10_17 = 0;
		uint8_t isr_type_18_25 = 0;
		uint8_t isr_type_26_33 = 0;
		uint8_t isr_type_34_41 = 0;
		uint8_t isr_type_42_49 = 0;
		uint8_t isr_type_50_57 = 0;
		
	
		//define individual ISR's
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF2A
		isr_t Ocf2AIsr = {nullptr};
		ISR(TIMER2_COMPA_vect) //#14
		{
			if (Ocf2AIsr.listener != nullptr)
			{
				if (isr_type_10_17 & (1 << 4)) //listener?
				{
					Ocf2AIsr.listener->trigger();
				}
				else //callback?
				{
					Ocf2AIsr.callback();
				}
			}
		}
		#endif
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF2B
		isr_t Ocf2BIsr = {nullptr};
		ISR(TIMER2_COMPB_vect) //#15
		{
			if (Ocf2BIsr.listener != nullptr)
			{
				if (isr_type_10_17 & (1 << 5)) //listener?
				{
					Ocf2BIsr.listener->trigger();
				}
				else //callback?
				{
					Ocf2BIsr.callback();
				}
			}
		}
		#endif
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV2
		isr_t Tov2Isr = {nullptr};
		ISR(TIMER2_OVF_vect) //#16
		{
			if (Tov2Isr.listener != nullptr)
			{
				if (isr_type_10_17 & (1 << 6)) //listener?
				{
					Tov2Isr.listener->trigger();
				}
				else //callback?
				{
					Tov2Isr.callback();
				}
			}
		}
		#endif
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF1
		isr_t Icf1Isr = {nullptr};
		ISR(TIMER1_CAPT_vect) //#17
		{
			if (Icf1Isr.listener != nullptr)
			{
				if (isr_type_10_17 & (1 << 7)) //listener?
				{
					Icf1Isr.listener->trigger();
				}
				else //callback?
				{
					Icf1Isr.callback();
				}
			}
		}
		#endif
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF1A
		isr_t Ocf1AIsr = {nullptr};
		ISR(TIMER1_COMPA_vect) //#18
		{
			if (Ocf1AIsr.listener != nullptr)
			{
				if (isr_type_18_25 & (1 << 0)) //listener?
				{
					Ocf1AIsr.listener->trigger();
				}
				else //callback?
				{
					Ocf1AIsr.callback();
				}
			}
		}
		#endif
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF1B
		isr_t Ocf1BIsr = {nullptr};
		ISR(TIMER1_COMPB_vect) //#19
		{
			if (Ocf1BIsr.listener != nullptr)
			{
				if (isr_type_18_25 & (1 << 1)) //listener?
				{
					Ocf1BIsr.listener->trigger();
				}
				else //callback?
				{
					Ocf1BIsr.callback();
				}
			}
		}
		#endif
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF1C
		isr_t Ocf1CIsr = {nullptr};
		ISR(TIMER1_COMPC_vect) //#20
		{
			if (Ocf1CIsr.listener != nullptr)
			{
				if (isr_type_18_25 & (1 << 2)) //listener?
				{
					Ocf1CIsr.listener->trigger();
				}
				else //callback?
				{
					Ocf1CIsr.callback();
				}
			}
		}
		#endif
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV1
		isr_t Tov1Isr = {nullptr};
		ISR(TIMER1_OVF_vect) //#21
		{
			if (Tov1Isr.listener != nullptr)
			{
				if (isr_type_18_25 & (1 << 3)) //listener?
				{
					Tov1Isr.listener->trigger();
				}
				else //callback?
				{
					Tov1Isr.callback();
				}
			}
		}
		#endif
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF0A
		isr_t Ocf0AIsr = {nullptr};
		ISR(TIMER0_COMPA_vect) //#22
		{
			if (Ocf0AIsr.listener != nullptr)
			{
				if (isr_type_18_25 & (1 << 4)) //listener?
				{
					Ocf0AIsr.listener->trigger();
				}
				else //callback?
				{
					Ocf0AIsr.callback();
				}
			}
		}
		#endif
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF0B
		isr_t Ocf0BIsr = {nullptr};
		ISR(TIMER0_COMPB_vect) //#23
		{
			if (Ocf0BIsr.listener != nullptr)
			{
				if (isr_type_18_25 & (1 << 5)) //listener?
				{
					Ocf0BIsr.listener->trigger();
				}
				else //callback?
				{
					Ocf0BIsr.callback();
				}
			}
		}
		#endif
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV0
		isr_t Tov0Isr = {nullptr};
		ISR(TIMER0_OVF_vect) //#24
		{
			if (Tov0Isr.listener != nullptr)
			{
				if (isr_type_18_25 & (1 << 6)) //listener?
				{
					Tov0Isr.listener->trigger();
				}
				else //callback?
				{
					Tov0Isr.callback();
				}
			}
		}
		#endif
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF3
		isr_t Icf3Isr = {nullptr};
		ISR(TIMER3_CAPT_vect) //#32
		{
			if (Icf3Isr.listener != nullptr)
			{
				if (isr_type_26_33 & (1 << 6)) //listener?
				{
					Icf3Isr.listener->trigger();
				}
				else //callback?
				{
					Icf3Isr.callback();
				}
			}
		}
		#endif
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF3A
		isr_t Ocf3AIsr = {nullptr};
		ISR(TIMER3_COMPA_vect) //#33
		{
			if (Ocf3AIsr.listener != nullptr)
			{
				if (isr_type_26_33 & (1 << 7)) //listener?
				{
					Ocf3AIsr.listener->trigger();
				}
				else //callback?
				{
					Ocf3AIsr.callback();
				}
			}
		}
		#endif
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF3B
		isr_t Ocf3BIsr = {nullptr};
		ISR(TIMER3_COMPB_vect) //#34
		{
			if (Ocf3BIsr.listener != nullptr)
			{
				if (isr_type_34_41 & (1 << 0)) //listener?
				{
					Ocf3BIsr.listener->trigger();
				}
				else //callback?
				{
					Ocf3BIsr.callback();
				}
			}
		}
		#endif
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF3C
		isr_t Ocf3CIsr = {nullptr};
		ISR(TIMER3_COMPC_vect) //#35
		{
			if (Ocf3CIsr.listener != nullptr)
			{
				if (isr_type_34_41 & (1 << 1)) //listener?
				{
					Ocf3CIsr.listener->trigger();
				}
				else //callback?
				{
					Ocf3CIsr.callback();
				}
			}
		}
		#endif
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV3
		isr_t Tov3Isr = {nullptr};
		ISR(TIMER3_OVF_vect) //#36
		{
			if (Tov3Isr.listener != nullptr)
			{
				if (isr_type_34_41 & (1 << 2)) //listener?
				{
					Tov3Isr.listener->trigger();
				}
				else //callback?
				{
					Tov3Isr.callback();
				}
			}
		}
		#endif
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF4
		isr_t Icf4Isr = {nullptr};
		ISR(TIMER4_CAPT_vect) //#42
		{
			if (Icf4Isr.listener != nullptr)
			{
				if (isr_type_42_49 & (1 << 0)) //listener?
				{
					Icf4Isr.listener->trigger();
				}
				else //callback?
				{
					Icf4Isr.callback();
				}
			}
		}
		#endif
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF4A
		isr_t Ocf4AIsr = {nullptr};
		ISR(TIMER4_COMPA_vect) //#43
		{
			if (Ocf4AIsr.listener != nullptr)
			{
				if (isr_type_42_49 & (1 << 1)) //listener?
				{
					Ocf4AIsr.listener->trigger();
				}
				else //callback?
				{
					Ocf4AIsr.callback();
				}
			}
		}
		#endif
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF4B
		isr_t Ocf4BIsr = {nullptr};
		ISR(TIMER4_COMPB_vect) //#44
		{
			if (Ocf4BIsr.listener != nullptr)
			{
				if (isr_type_42_49 & (1 << 2)) //listener?
				{
					Ocf4BIsr.listener->trigger();
				}
				else //callback?
				{
					Ocf4BIsr.callback();
				}
			}
		}
		#endif
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF4C
		isr_t Ocf4CIsr = {nullptr};
		ISR(TIMER4_COMPC_vect) //#45
		{
			if (Ocf4CIsr.listener != nullptr)
			{
				if (isr_type_42_49 & (1 << 3)) //listener?
				{
					Ocf4CIsr.listener->trigger();
				}
				else //callback?
				{
					Ocf4CIsr.callback();
				}
			}
		}
		#endif
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV4
		isr_t Tov4Isr = {nullptr};
		ISR(TIMER4_OVF_vect) //#46
		{
			if (Tov4Isr.listener != nullptr)
			{
				if (isr_type_42_49 & (1 << 4)) //listener?
				{
					Tov4Isr.listener->trigger();
				}
				else //callback?
				{
					Tov4Isr.callback();
				}
			}
		}
		#endif
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF5
		isr_t Icf5Isr = {nullptr};
		ISR(TIMER5_CAPT_vect) //#47
		{
			if (Icf5Isr.listener != nullptr)
			{
				if (isr_type_42_49 & (1 << 5)) //listener?
				{
					Icf5Isr.listener->trigger();
				}
				else //callback?
				{
					Icf5Isr.callback();
				}
			}
		}
		#endif
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF5A
		isr_t Ocf5AIsr = {nullptr};
		ISR(TIMER5_COMPA_vect) //#48
		{
			if (Ocf5AIsr.listener != nullptr)
			{
				if (isr_type_42_49 & (1 << 6)) //listener?
				{
					Ocf5AIsr.listener->trigger();
				}
				else //callback?
				{
					Ocf5AIsr.callback();
				}
			}
		}
		#endif
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF5B
		isr_t Ocf5BIsr = {nullptr};
		ISR(TIMER5_COMPB_vect) //49
		{
			if (Ocf5BIsr.listener != nullptr)
			{
				if (isr_type_42_49 & (1 << 7)) //listener?
				{
					Ocf5BIsr.listener->trigger();
				}
				else //callback?
				{
					Ocf5BIsr.callback();
				}
			}
		}
		#endif
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF5C
		isr_t Ocf5CIsr = {nullptr};
		ISR(TIMER5_COMPC_vect) //#50
		{
			if (Ocf5CIsr.listener != nullptr)
			{
				if (isr_type_50_57 & (1 << 0)) //listener?
				{
					Ocf5CIsr.listener->trigger();
				}
				else //callback?
				{
					Ocf5CIsr.callback();
				}
			}
		}
		#endif
		#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV5
		isr_t Tov5Isr = {nullptr};
		ISR(TIMER5_OVF_vect) //#51
		{
			if (Tov5Isr.listener != nullptr)
			{
				if (isr_type_50_57 & (1 << 1)) //listener?
				{
					Tov5Isr.listener->trigger();
				}
				else //callback?
				{
					Tov5Isr.callback();
				}
			}
		}
		#endif
	}


	//Set_ISR functions
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF2A //#14
	void setOCF2A(Listener* listener)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_10_17 |= (1 << 4);
			Ocf2AIsr.listener = listener;
		}
	}
	void setOCF2A(void (*callback)())
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_10_17 &= ~(1 << 4);
			Ocf2AIsr.callback = callback;
		}
	}
	#endif
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF2B //#15
	void setOCF2B(Listener* listener)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_10_17 |= (1 << 5);
			Ocf2BIsr.listener = listener;
		}
	}
	void setOCF2B(void (*callback)())
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_10_17 &= ~(1 << 5);
			Ocf2BIsr.callback = callback;
		}
	}
	#endif
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV2 //#16
	void setTOV2(Listener* listener)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_10_17 |= (1 << 6);
			Tov2Isr.listener = listener;
		}
	}
	void setTOV2(void (*callback)())
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_10_17 &= ~(1 << 6);
			Tov2Isr.callback = callback;
		}
	}
	#endif
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF1 //#17
	void setICF1(Listener* listener)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_10_17 |= (1 << 7);
			Icf1Isr.listener = listener;
		}
	}
	void setICF1(void (*callback)())
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_10_17 &= ~(1 << 7);
			Icf1Isr.callback = callback;
		}
	}
	#endif
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF1A //#18
	void setOCF1A(Listener* listener)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_18_25 |= (1 << 0);
			Ocf1AIsr.listener = listener;
		}
	}
	void setOCF1A(void (*callback)())
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_18_25 &= ~(1 << 0);
			Ocf1AIsr.callback = callback;
		}
	}
	#endif
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF1B //#19
	void setOCF1B(Listener* listener)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_18_25 |= (1 << 1);
			Ocf1BIsr.listener = listener;
		}
	}
	void setOCF1B(void (*callback)())
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_18_25 &= ~(1 << 1);
			Ocf1BIsr.callback = callback;
		}
	}
	#endif
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF1C //#20
	void setOCF1C(Listener* listener)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_18_25 |= (1 << 2);
			Ocf1CIsr.listener = listener;
		}
	}
	void setOCF1C(void (*callback)())
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_18_25 &= ~(1 << 2);
			Ocf1CIsr.callback = callback;
		}
	}
	#endif
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV1 //#21
	void setTOV1(Listener* listener)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_18_25 |= (1 << 3);
			Tov1Isr.listener = listener;
		}
	}
	void setTOV1(void (*callback)())
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_18_25 &= ~(1 << 3);
			Tov1Isr.callback = callback;
		}
	}
	#endif
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF0A //#22
	void setOCF0A(Listener* listener)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_18_25 |= (1 << 4);
			Ocf0AIsr.listener = listener;
		}
	}
	void setOCF0A(void (*callback)())
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_18_25 &= ~(1 << 4);
			Ocf0AIsr.callback = callback;
		}
	}
	#endif
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF0B //#23
	void setOCF0B(Listener* listener)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_18_25 |= (1 << 5);
			Ocf0BIsr.listener = listener;
		}
	}
	void setOCF0B(void (*callback)())
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_18_25 &= ~(1 << 5);
			Ocf0BIsr.callback = callback;
		}
	}
	#endif
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV0 //#24
	void setTOV0(Listener* listener)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_18_25 |= (1 << 6);
			Tov0Isr.listener = listener;
		}
	}
	void setTOV0(void (*callback)())
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_18_25 &= ~(1 << 6);
			Tov0Isr.callback = callback;
		}
	}
	#endif
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF3 //#32
	void setICF3(Listener* listener)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_26_33 |= (1 << 6);
			Icf3Isr.listener = listener;
		}
	}
	void setICF3(void (*callback)())
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_26_33 &= ~(1 << 6);
			Icf3Isr.callback = callback;
		}
	}
	#endif
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF3A //#33
	void setOCF3A(Listener* listener)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_26_33 |= (1 << 7);
			Ocf3AIsr.listener = listener;
		}
	}
	void setOCF3A(void (*callback)())
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_26_33 &= ~(1 << 7);
			Ocf3AIsr.callback = callback;
		}
	}
	#endif
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF3B //#34
	void setOCF3B(Listener* listener)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_34_41 |= (1 << 0);
			Ocf3BIsr.listener = listener;
		}
	}
	void setOCF3B(void (*callback)())
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_34_41 &= ~(1 << 0);
			Ocf3BIsr.callback = callback;
		}
	}
	#endif
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF3C //#35
	void setOCF3C(Listener* listener)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_34_41 |= (1 << 1);
			Ocf3CIsr.listener = listener;
		}
	}
	void setOCF3C(void (*callback)())
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_34_41 &= ~(1 << 1);
			Ocf3CIsr.callback = callback;
		}
	}
	#endif
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV3 //#36
	void setTOV3(Listener* listener)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_34_41 |= (1 << 2);
			Tov3Isr.listener = listener;
		}
	}
	void setTOV3(void (*callback)())
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_34_41 &= ~(1 << 2);
			Tov3Isr.callback = callback;
		}
	}
	#endif
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF4 //#42
	void setICF4(Listener* listener)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_42_49 |= (1 << 0);
			Icf4Isr.listener = listener;
		}
	}
	void setICF4(void (*callback)())
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_42_49 &= ~(1 << 0);
			Icf4Isr.callback = callback;
		}
	}
	#endif
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF4A //#43
	void setOCF4A(Listener* listener)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_42_49 |= (1 << 1);
			Ocf4AIsr.listener = listener;
		}
	}
	void setOCF4A(void (*callback)())
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_42_49 &= ~(1 << 1);
			Ocf4AIsr.callback = callback;
		}
	}
	#endif
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF4B //#44
	void setOCF4B(Listener* listener)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_42_49 |= (1 << 2);
			Ocf4BIsr.listener = listener;
		}
	}
	void setOCF4B(void (*callback)())
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_42_49 &= ~(1 << 2);
			Ocf4BIsr.callback = callback;
		}
	}
	#endif
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF4C //#45
	void setOCF4C(Listener* listener)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_42_49 |= (1 << 3);
			Ocf4CIsr.listener = listener;
		}
	}
	void setOCF4C(void (*callback)())
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_42_49 &= ~(1 << 3);
			Ocf4CIsr.callback = callback;
		}
	}
	#endif
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV4 //#46
	void setTOV4(Listener* listener)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_42_49 |= (1 << 4);
			Tov4Isr.listener = listener;
		}
	}
	void setTOV4(void (*callback)())
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_42_49 &= ~(1 << 4);
			Tov4Isr.callback = callback;
		}
	}
	#endif
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_ICF5 //#47
	void setICF5(Listener* listener)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_42_49 |= (1 << 5);
			Icf5Isr.listener = listener;
		}
	}
	void setICF5(void (*callback)())
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_42_49 &= ~(1 << 5);
			Icf5Isr.callback = callback;
		}
	}
	#endif
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF5A //#48
	void setOCF5A(Listener* listener)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_42_49 |= (1 << 6);
			Ocf5AIsr.listener = listener;
		}
	}
	void setOCF5A(void (*callback)())
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_42_49 &= ~(1 << 6);
			Ocf5AIsr.callback = callback;
		}
	}
	#endif
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF5B //#49
	void setOCF5B(Listener* listener)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_42_49 |= (1 << 7);
			Ocf5BIsr.listener = listener;
		}
	}
	void setOCF5B(void (*callback)())
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_42_49 &= ~(1 << 7);
			Ocf5BIsr.callback = callback;
		}
	}
	#endif
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF5C //#50
	void setOCF5C(Listener* listener)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_50_57 |= (1 << 0);
			Ocf5CIsr.listener = listener;
		}
	}
	void setOCF5C(void (*callback)())
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_50_57 &= ~(1 << 0);
			Ocf5CIsr.callback = callback;
		}
	}
	#endif
	#if ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV5 //#51
	void setTOV5(Listener* listener)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_50_57 |= (1 << 1);
			Tov5Isr.listener = listener;
		}
	}
	void setTOV5(void (*callback)())
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			isr_type_50_57 &= ~(1 << 1);
			Tov5Isr.callback = callback;
		}
	}
	#endif
}
