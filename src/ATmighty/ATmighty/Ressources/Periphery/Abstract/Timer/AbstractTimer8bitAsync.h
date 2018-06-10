/*
 * This headerfile represents a common interface for all abstract 8bit Timers which support asynchronous operation.
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_TIMER_ABSTRACTTIMER8BITASYNC_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_TIMER_ABSTRACTTIMER8BITASYNC_H_


#include "ATmighty/Ressources/Periphery/Abstract/Timer/AbstractTimer8bit.h"


///This class represents a common interface for all abstract 8bit Timers which support asynchronous operation
class AbstractTimer8bitAsync : public AbstractTimer8bit
{
	friend class AbstractHardwareManager;

	protected:
		/// Default Constructor - Can only be called by friends
		inline AbstractTimer8bitAsync() {}

		/// Virtual Destructor
		virtual ~AbstractTimer8bitAsync() {}

	public:
		/// enum-type definition for asynchronous modes of operation
		enum AsyncMode : uint8_t
		{
			Synchronous = 0,
			AsynchronousCrystal = 1,
			AsynchronousClock = 3
		};

		/*!
		 * Sets the asynchronous mode of operation.
		 * \param mode the value of bits "EXCLK" and "ASx" within the ASSR-register. Possible values are:
		 * 0: Normal, synchronous operation using the internal clock (default)
		 * 1: Asynchronous operation using a crystal-oscillator between the TOSC1 and TOSC2 pins.
		 * 3: Asynchronous operation using a digital clock-signal on pin TOSC1
		 * When the mode != 0 is set, the timer will allocate both of the asynchronous Timer-Oscillator-Pins (TOSC1 & TOSC2).
		 * Freeing the affected IoPins afterwards is only possible by freeing the entire timer.
		 * \returns 0 on success, <0 if mode was invalid, >0 if the Timer-Oscillator-IoPins could not be allocated
		 */
		virtual int8_t setAsyncMode(AsyncMode mode) = 0;

		/*!
		 * Returns all the "Update-Busy"-flags from the ASSR-register as one uint8_t.
		 * The flags are set '1' when a new value gets written into one of the timers registers in asynchronous operation-mode.
		 * The flags get cleared to '0' after the asynchronous register was updated from the temporary storage register.
		 * A '0'-flag means that the register is ready to be set with a new value. If a register is written in asynchronous operation-mode
		 * while the connected "Update-Busy"-flag is set, a Warning is messaged by the timer, but the value gets written into the register
		 * anyways. As this can lead to corrupted registers and unwanted side-effects, it is the users responsibility to check the flags
		 * before performing a register-write operation (see return-value description) in asynchronous mode.
		 * \returns The 5 least-significant bits of the returned uint8_t represent a flag each. They are connected to the registers a following:
		 * Bit 0: TCCRxB	[affected by: setWGM(), setPrescalar() and forceOutputCompare()]
		 * Bit 1: TCCRxA	[affected by: setWGM(), setCOMx()]
		 * Bit 2: OCRxB		[affected by: setOCRx()]
		 * Bit 3: OCRxA		[affected by: setOCRx()]
		 * Bit 4: TCNTx		[affected by: setCounter()]
		 */
		virtual uint8_t getAsyncUpdateBusyFlags() = 0;
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_TIMER_ABSTRACTTIMER8BITASYNC_H_ */
