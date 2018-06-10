/*!
 * This headerfile represents an abstract wrapper for physical asynchronous Timer2 of the ATmega2560
 * This headerfile is µC-dependent, it should only be included indirectly via "Timer.h".
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_TIMER_ATMEGA2560_ABSTRACTTIMER2_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_TIMER_ATMEGA2560_ABSTRACTTIMER2_H_


#include <ATmighty/Ressources/Periphery/Abstract/Timer/AbstractTimer8bitAsync.h>


//forward declarations
class Timer2;
template<char PortChar, uint8_t PinNumber> class SpecificIoPin;


/// This class abstractly wraps physical Timer0.
class AbstractTimer2 : public AbstractTimer8bitAsync
{
	friend class AbstractHardwareManager;

	private:
		///static owner-id, as a specific Timer should behave like a Singleton and can only be owned once.
		static int8_t Owner;

		///A reference to the physical Timer0-instance wrapped by this AbstractTimer0
		Timer2* physicalReference;

		///References to the abstract IoPins used as outputs by this timer. Implementing them static saves dynamically allocated memory.
		static SpecificIoPin<'B',4>* OutputA;
		static SpecificIoPin<'H',6>* OutputB;
		static SpecificIoPin<'G',4>* Tosc1;
		static SpecificIoPin<'G',3>* Tosc2;

		///Default Constructor
		inline AbstractTimer2() : physicalReference(nullptr) {}

		///Destructor
		~AbstractTimer2() {}

		/*!
		 * Tries to allocate this abstract hardware, initializes physical hardware dependencies.
		 * This routine can only be called by friends.
		 * \param ownerId The owner-id of the caller who wants to allocate this abstract hardware
		 * \returns 0 on success, >0 if this abstract hardware is already in use or <0 if the physical hardware dependencies could not be resolved.
		 */
		int8_t init(int8_t ownerId);

		/*!
		 * Frees this abstract hardware and the allocated physical hardware dependencies.
		 * This routine can only be called by friends.
		 */
		void exit();

		/// returns the current owner of this abstract hardware
		inline int8_t getOwner() {return Owner;}

	public:
		/*!
		 * Sets the Waveform-Generation-Mode of this timer.
		 * \param value the value to set into the WGMn-bits, range 0b000 - 0b111. (check datasheet of µC for more informations)
		 * \returns 0 on success, <0 if value was invalid.
		 */
		int8_t setWGM(uint8_t value);

		/*!
		 * Sets the prescale-factor of this timer in terms of potencys of two. Setting this value will start the timer.
		 * \param potency the number of two-potencys which each half the clock-frequency to be used by this timer:
		 * For example: potency 6 => 2^6 = 64 = prescalar. range 0-10.
		 * This Timer supports potencys 0[1], 3[8], 5[32], 6[64], 7[128], 8[256], 10[1024].
		 * You may use the predefined enum-values of type "Prescale" for easier usage.
		 * \returns 0 on success, <0 if value was invalid.
		 */
		int8_t setPrescalar(uint8_t potency);

		/// Stops the timer entirely by setting the prescale-factor to 0.
		void stop();

		/// Sets the current value of the timer-counter. Be aware of possible side-effects, check datasheet of µC for more details.
		void setCounter(uint8_t value);

		/// Returns the current value of the timer-counter.
		uint8_t getCounter();

		/*!
		 * Sets the Output-Compare-Register value of output-channel x.
		 * The result of this operation depends of the current mode of this timer (specified by the Waveform-Generation-Mode bits).
		 * \param value the value to set into the OCRnx-register.
		 * \param channel the output channel x which shall be affected by this operation. (upper-case, 'A' / 'B')
		 * \returns 0 on sucess, >0 if channel was invalid.
		 */
		int8_t setOCRx(uint8_t value, char channel);

		/*!
		 * Forces an output-compare-match on an specified output-channel when in non-PWM-mode.
		 * This will effect the output of this channel as specified by the Compare-Match-Output-Mode,
		 * but will not trigger any interrupts nor will it clear the timer in CTC-mode!
		 * \param channel the output channel which shall be affected by this operation. (upper-case, 'A' / 'B')
		 * \returns 0 on sucess, >0 if channel was invalid, <0 if operation is invalid because the timer is currently in PWM-mode.
		 */
		int8_t forceOutputCompare(char channel);

		/*!
		 * Sets the Compare-Match-Output-Mode value of output-channel x.
		 * Setting this value != 0 will allocate the connected IoPin to block it from other users.
		 * (Freeing the IoPin afterwards is only possible by freeing the entire timer, though access to the IoPin can be gained via "getOutputPin()")
		 * In case the connected IoPin could not be allocated, no value > 0 will be set into the register!
		 * \param value the value to set into the COMnx-bits, range 0b00 - 0b11. (0b00 = no output, check datasheet of µC for other bit-combinations)
		 * \param channel the output channel x which shall be affected by this operation. (upper-case, 'A', 'B',...)
		 * \returns 0 on sucess, <0 if value was invalid, 1 if channel was invalid, >1 if the connected IoPin could not be allocated.
		 */
		int8_t setCOMx(uint8_t value, char channel);

		/*!
		 * Gets an abstract instance of the IoPin used as output by this timer on a specified channel.
		 * This can be used to manually interact with the output-pins of this timer, which is for example necessary
		 * to enable/disable the actual physical output of this timer by setting the data-direction of the IoPin respectively.
		 * \param channel the output channel which shall be affected by this operation. (upper-case, 'A' / 'B')
		 * \returns a pointer to the used IoPin on success, nullptr if the channel was invalid or the IoPin could not be allocated.
		 */
		IoPin* getOutputPin(char channel);

		/*!
		 * This function will do nothing, as this timer does not support an external clock-input.
		 * \returns 1 (Timer does not support external clocking)
		 */
		inline int8_t setExtClockInput(ExtClkTrigger trigger) {return 1;}

		/*!
		 * Gets an abstract instance of the IoPin used as asynchronous input by this timer. (TOSC1).
		 * This method will allocate the asynchronous Timer-Oscillator-Pin1 (TOSC1).
		 * Freeing the IoPin afterwards is only possible by freeing the entire timer.
		 * This method can be useful to gain access to the asynchronous input-pin while the timer is NOT in asynchronous operation mode.
		 * \returns a pointer to the used IoPin on success, nullptr if the IoPin could not be allocated.
		 */
		IoPin* getExtClkPin();

		/*!
		 * Enables/disables the Output-Compare-Match interrupt. This will always clear the corresponding interrupt-flag.
		 * \param value true to enable the interrupt, false to disable the interrupt.
		 * \param channel the Compare-Match channel which shall be affected by this operation. (upper-case, 'A' / 'B')
		 * Will do nothing if the channel is invalid
		 */
		void enableOutputCompareInterrupt(bool value, char channel);

		/*!
		 * Enables/disables the Timer-Overflow interrupt. This will always clear the corresponding interrupt-flag.
		 * \param value true to enable the interrupt, false to disable the interrupt.
		 */
		void enableTimerOverflowInterrupt(bool value);

		/*!
		 * Sets a Listener which gets triggered when a Compare-Match-Interrupt occurs for a given channel. (Will do nothing if the channel is invalid)
		 * Call this method with argument isr==nullptr to remove the isr-listener for this interrupt.
		 * This operation will only have an effect when the corresponding Interrupt-Management by ATmighty is enabled in the Interrupt-Config.h
		 * \param isr the Listener-object which will be triggered to serve as interrupt-service-routine.
		 * \param channel the Compare-Match channel which shall be affected by this operation. (upper-case, 'A' / 'B')
		 */
		void setOutputCompareISR(Listener* isr, char channel);

		/*!
		 * Sets a Listener which gets triggered when a Timer-Overflow-Interrupt occurs.
		 * Call this method with argument isr==nullptr to remove the isr-listener for this interrupt.
		 * This operation will only have an effect when the corresponding Interrupt-Management by ATmighty is enabled in the Interrupt-Config.h
		 * \param isr the Listener-object which will be triggered to serve as interrupt-service-routine.
		 */
		void setTimerOverflowISR(Listener* isr);

		/// returns the total number of PWM-output-channels driven by this timer (upper-case, 'A' / 'B')
		inline uint8_t getNumberOfChannels() {return 2;}

		/*!
		 * When in synchronous operation-mode, this method returns the base-clock frequency which drives this timer in Hz.
		 * (Will be processor-speed, 16.000.000 for Arduino).
		 * When in asynchronous operation-mode, this method will return 0, as the current frequency is unknown.
		 */
		uint32_t getBaseFrequency();

		/// Returns the corresponding letter (upper-case) associated with this Timer.
		inline char getCharCode() {return '2';}

		/*!
		 * Sets the asynchronous mode of operation.
		 * \param mode the value of bits "EXCLK" and "AS2" within the ASSR-register. Possible values are:
		 * 0: Normal, synchronous operation using the internal clock (default)
		 * 1: Asynchronous operation using a crystal-oscillator between the TOSC1 and TOSC2 pins.
		 * 3: Asynchronous operation using a digital clock-signal on pin TOSC1
		 * When the mode != 0 is set, the timer will allocate both of the asynchronous Timer-Oscillator-Pins (TOSC1 & TOSC2).
		 * Freeing the affected IoPins afterwards is only possible by freeing the entire timer.
		 * \returns 0 on success, <0 if mode was invalid, >0 if the Timer-Oscillator-IoPins could not be allocated
		 */
		int8_t setAsyncMode(AsyncMode mode);

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
		inline uint8_t getAsyncUpdateBusyFlags() {return (ASSR & 0x1F);}
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_TIMER_ATMEGA2560_ABSTRACTTIMER2_H_ */
