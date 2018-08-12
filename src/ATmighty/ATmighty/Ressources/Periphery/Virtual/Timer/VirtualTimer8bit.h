/*
 * This headerfile is µC-independent.
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_VIRTUAL_TIMER_VIRTUALTIMER8BIT_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_VIRTUAL_TIMER_VIRTUALTIMER8BIT_H_


#include "ATmighty/Ressources/Periphery/Abstract/Timer.h"
#include "ATmighty/Ressources/Interrupts/InterruptManager.h"


class VirtualTimer8bit final : public Timer8bit
{
	template<class Timer> friend class VirtualTimerPool;

	private:
		//typedef for structure which holds all the per-channel-data (reduces dynamic memory allocation overhead)
		typedef struct perChannel
		{
			///The virtual output-compare register for each channel
			uint8_t ocrx;

			///The virtual output-compare register for each channel
			uint8_t ocrxBuffer;

			///ISR (Listener or callback-function) for each channel
			interruptHandler_t outputCompareMatchISR;
		} channelData_t;

		///An Array which hold the per-channel-data
		channelData_t* channelData;

		///the base-frequency this timer runs with in Hz (provided by Constructor)
		uint16_t baseFrequency;

		///the timer-counter value of this virtual timer
		uint8_t tcnt;

		///the waveform-generation-mode of this virtual timer
		uint8_t wgm;

		///the currently configured prescalar value
		uint16_t prescalar;

		///the counter which represents the actual prescaling
		uint16_t prescaleCounter;

		///the output-compare-modes for each channel. (2bits per channel)
		uint8_t comx;

		///the total number of channels of this virtual timer
		uint8_t channels;

		/*!
		 * Bitmask holds the enabled/disabled state for each virtual interrupt.
		 * OutputCompareMatch-interrupt-enable flags for each channel are on the LSB's,
		 * TimerOverflow-interrupt-enable flag is the MSB.
		 * 1 = Enabled / 0 = Disabled.
		 */
		uint8_t interruptEnFlags;

		/*!
		 * Bitmask holds the type of isr-routine for each virtual interrupt.
		 * OutputCompareMatch-isr-types for each channel are on the LSB's,
		 * TimerOverflow-isr-type is the MSB.
		 * 1 = Listener / 0 = function-pointer.
		 */
		uint8_t isrTypeMask;

		///The ISR for the timer-overflow event
		interruptHandler_t timerOverflowISR;

		///An array of Output-Pins for each channel of this virtual timer. (All or none)
		IoPin** outputPins;

		/*!
		 * Holds some flags which are necessary to represent this timers state correctly.
		 * Bit0 - Running: 1 = this timer is running (acts on calls to "tick()"), 0 = this timer is stopped.
		 * Bit1 - PWM-CountDirection: 1 = The counter is decrementing (in Phase-correct-PWM-Mode), 0 = normal incrementing counter.
		 * Bit2 - Need to update OCRx-registers at next update-condition for current wgm-mode.
		 * Bit3 - skipNextCount: Don't count at the next execution of "tick()".
		 * Bit4 - BypassPrescalar: set this Bit 1 to execute a tick()-Method immediately without waiting for the next prescalar-match
		 * Bit5-7 - VirtualTimerPool-Index: The index of this virtual timer within the corresponding VirtualTimerPool.
		 */
		uint8_t statusFlags;

		/*!
		 * Minimalistic Constructor
		 * \param baseFrequency the frequency this virtual timer will be driven in Hz. It is important to provide the exact frequency
		 * at which the "tick()"-method will be called to ensure that this virtual timer behaves for every user as expected.
		 * \param channels the number of output-compare-channels this virtual timer shall have (1-4, default = 2)
		 */
		VirtualTimer8bit(uint16_t baseFrequency, uint8_t channels = 2);

		/*!
		 * "PWM" Constructor
		 * \param baseFrequency the frequency this virtual timer will be driven in Hz. It is important to provide the exact frequency
		 * at which the "tick()"-method will be called to ensure that this virtual timer behaves for every user as expected.
		 * \param outputs An array of IoPins used as output-pins by this virtual timer. The size of the array must be the same as the number
		 * of channels! (Provide an output IoPin for all channels, or use the other Constructor and have no outputs at all)
		 * \param channels the number of output-compare-channels this virtual timer shall have (1-4)
		 */
		VirtualTimer8bit(uint16_t baseFrequency, IoPin** outputs, uint8_t channels);

		///Deconstructor
		~VirtualTimer8bit();

		/*!
		 * Sets an Index (0-7) this virtual Timer has within its VirtualTimerPool-environment.
		 * \param virtualTimerPoolIndex The index of this virtual timer within the corresponding VirtualTimerPool
		 */
		inline void setVirtualTimerPoolIndex(uint8_t virtualTimerPoolIndex)
		{
			statusFlags = ((statusFlags & ~(7 << 5)) | (virtualTimerPoolIndex << 5));
		}

		///returns the value stored as virtualTimerPoolIndex (0-7)
		inline uint8_t getVirtualTimerPoolIndex()
		{
			return (statusFlags >> 5);
		}

		///Gets called to increment TCNT, represents a virtual clock-input
		void tick();

		///Gets called when the Timer reaches one of the OCRx-values
		void compareMatchEvent(uint8_t channel);

		///Gets called when the timer reaches its TimerOverflow-condition
		void timerOverflowEvent();

		///Updates all ocrx-registers from ocrxBuffers
		void updateOCRx();

	public:
		/*!
		 * Sets the Waveform-Generation-Mode of this timer.
		 * \param value the value to set into the WGMn-bits, range 0b000 - 0b111.
		 * Same values as described in datasheet for normal AVR 8bit-Timers:
		 * 0 - Normal Mode
		 * 1 - Phase-correct PWM with Top = 0xFF
		 * 2 - CTC
		 * 3 - Fast PWM with Top = 0xFF
		 * 5 - Phase-correct PWM with Top = OCRA
		 * 7 - Fast PWM with Top = OCRA
		 * \returns 0 on success, <0 if value was invalid.
		 */
		int8_t setWGM(uint8_t value);

		/*!
		 * Sets the prescale-factor of this timer in terms of potencys of two. Setting this value will start the timer.
		 * \param potency the number of two-potencys which each half the clock-frequency to be used by this timer:
		 * For example: potency 6 => 2^6 = 64 = prescalar. range 0-10.
		 * This virtual timer supports potencys 0-15 (1 - 32768)
		 * \returns 0 on success, <0 if value was invalid.
		 */
		int8_t setPrescalar(uint8_t potency);

		/// Stops the timer.
		inline void stop() {statusFlags &= ~1; prescaleCounter = 0;}

		/// Sets the current value of the timer-counter.
		inline void setCounter(uint8_t value)
		{
			tcnt = value;
			statusFlags |= (3 << 3); //skipNextCount & BypassPrescalar
			tick();
		}

		/// Returns the current value of the timer-counter.
		inline uint8_t getCounter() {return tcnt;}

		/*!
		 * Sets the Output-Compare-Register value of output-channel x.
		 * The result of this operation depends of the current mode of this timer (specified by the Waveform-Generation-Mode bits).
		 * \param value the value to set into the OCRnx-register.
		 * \param channel the output channel x which shall be affected by this operation. (upper-case, 'A', 'B',...)
		 * \returns 0 on sucess, >0 if channel was invalid.
		 */
		int8_t setOCRx(uint8_t value, char channel);

		/*!
		 * Forces an output-compare-match on an specified output-channel when in non-PWM-mode.
		 * This will effect the output of this channel as specified by the Compare-Match-Output-Mode,
		 * but will not trigger any interrupts nor will it clear the timer in CTC-mode!
		 * \param channel the output channel which shall be affected by this operation. (upper-case, 'A', 'B',...)
		 * \returns 0 on sucess, >0 if channel was invalid, <0 if operation is invalid because the timer is currently in PWM-mode.
		 */
		int8_t forceOutputCompare(char channel);

		/*!
		 * Sets the Compare-Match-Output-Mode value of output-channel x.
		 * Setting this value != 0 will only work when this virtual timer was provided with output IoPins.
		 * \param value the value to set into the COMnx-bits, range 0b00 - 0b11.
		 * Same values as described in datasheet for normal AVR 8bit-Timers, check datasheet for more informations.
		 * \param channel the output channel x which shall be affected by this operation. (upper-case, 'A', 'B',...)
		 * \returns 0 on sucess, <0 if value was invalid, 1 if channel was invalid, >1 if the connected IoPin was not provided.
		 */
		int8_t setCOMx(uint8_t value, char channel);

		/*!
		 * Gets an abstract instance of the IoPin used as output by this timer on a specified channel.
		 * This will only work when this virtual timer was provided with output IoPins.
		 * \param channel the output channel which shall be affected by this operation. (upper-case, 'A', 'B',...)
		 * \returns a pointer to the used IoPin on success, nullptr if the channel was invalid or the IoPin was not provided.
		 */
		IoPin* getOutputPin(char channel);

		/*!
		 * Not supported by this virtual timer! Will always return 1.
		 */
		inline int8_t setExtClockInput(ExtClkTrigger trigger) {return 1;}

		/*!
		 * Not supported by this virtual timer! Will always return nullptr.
		 */
		inline IoPin* getExtClkPin() {return nullptr;}

		/*!
		 * Enables/disables the virtual Output-Compare-Match interrupt.
		 * \param value true to enable the interrupt, false to disable the interrupt.
		 * \param channel the Compare-Match channel which shall be affected by this operation. (upper-case, 'A', 'B',...)
		 * Will do nothing if the channel is invalid
		 */
		void enableOutputCompareInterrupt(bool value, char channel);

		/*!
		 * Enables/disables the virtual Timer-Overflow interrupt.
		 * \param value true to enable the interrupt, false to disable the interrupt.
		 */
		void enableTimerOverflowInterrupt(bool value);

		/*!
		 * Sets a Listener which gets triggered when a Compare-Match-Interrupt occurs for a given channel. (Will do nothing if the channel is invalid)
		 * Call this method with argument isr==nullptr to remove the isr-listener for this interrupt.
		 * \param isr the Listener-object which will be triggered to serve as interrupt-service-routine.
		 * \param channel the Compare-Match channel which shall be affected by this operation. (upper-case, 'A', 'B',...)
		 */
		void setOutputCompareISR(Listener* isr, char channel);

		/*!
		 * Sets a Callback-function which gets called when a Compare-Match-Interrupt occurs for a given channel. (Will do nothing if the channel is invalid)
		 * Call this method with argument isr==nullptr to remove the isr-callback for this interrupt.
		 * \param isr the functionpointer which will be called to serve as interrupt-service-routine.
		 * \param channel the Compare-Match channel which shall be affected by this operation. (upper-case, 'A', 'B',...)
		 */
		void setOutputCompareISR(void (*isr)(), char channel);

		/*!
		 * Sets a Listener which gets triggered when a Timer-Overflow-Interrupt occurs.
		 * Call this method with argument isr==nullptr to remove the isr-listener for this interrupt.
		 * \param isr the Listener-object which will be triggered to serve as interrupt-service-routine.
		 */
		void setTimerOverflowISR(Listener* isr);

		/*!
		 * Sets a Callback-function which gets called when a Timer-Overflow-Interrupt occurs.
		 * Call this method with argument isr==nullptr to remove the isr-callback for this interrupt.
		 * \param isr the functionpointer which will be called to serve as interrupt-service-routine.
		 */
		void setTimerOverflowISR(void (*isr)());

		/// returns the total number of PWM-output-channels driven by this timer (Channels = A,B,...)
		inline uint8_t getNumberOfChannels() {return channels;};

		/// returns the base-clock frequency which drives this timer in Hz.
		inline uint32_t getBaseFrequency() {return (uint32_t)baseFrequency;};
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_VIRTUAL_TIMER_VIRTUALTIMER8BIT_H_ */
