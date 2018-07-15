/*
 * This headerfile represents a common interface for all 8bit Timers. (abstract and virtual)
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_TIMER_TIMER8BIT_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_TIMER_TIMER8BIT_H_


#include <stdint.h>
#include <avr/pgmspace.h>

//forward-declarations
class Listener;
class IoPin;


///This class represents a pure virtual common interface for all 8bit Timers
class Timer8bit
{
	friend class AbstractHardwareManager;

	public:
		/// enum-type definition for easier use of the correct prescalar-values.
		enum Prescale : uint8_t
		{
			NoScale = 0,
			Scale8 = 3,
			Scale32 = 5,
			Scale64 = 6,
			Scale128 = 7,
			Scale256 = 8,
			Scale1024 = 10
		};

		/// enum-type definition for easier enabling of the external clock input with a specified trigger-type.
		enum ExtClkTrigger : uint8_t
		{
			RisingEdge = 7,
			FallingEdge = 6
		};

		/// Virtual Destructor
		virtual ~Timer8bit() {}

		/*!
		 * Sets the Waveform-Generation-Mode of this timer.
		 * \param value the value to set into the WGMn-bits, range 0b000 - 0b111. (check datasheet of µC for more informations)
		 * \returns 0 on success, <0 if value was invalid.
		 */
		virtual int8_t setWGM(uint8_t value) = 0;

		/*!
		 * Sets the prescale-factor of this timer in terms of potencys of two. Setting this value will start the timer.
		 * \param potency the number of two-potencys which each half the clock-frequency to be used by this timer:
		 * For example: potency 6 => 2^6 = 64 = prescalar. range 0-10.
		 * All 8-bit timers will support potencys 0[1], 3[8], 6[64], 8[256], 10[1024]. Asynchronous timers support additionally 5[32] and 7[128].
		 * You may use the predefined enum-values of type "Prescale" for easier usage.
		 * \returns 0 on success, <0 if value was invalid.
		 */
		virtual int8_t setPrescalar(uint8_t potency) = 0;

		/// Stops the timer entirely by setting the prescale-factor to 0.
		virtual void stop() = 0;

		/// Sets the current value of the timer-counter. Be aware of possible side-effects, check datasheet of µC for more details.
		virtual void setCounter(uint8_t value) = 0;

		/// Returns the current value of the timer-counter.
		virtual uint8_t getCounter() = 0;

		/*!
		 * Sets the Output-Compare-Register value of output-channel x.
		 * The result of this operation depends of the current mode of this timer (specified by the Waveform-Generation-Mode bits).
		 * \param value the value to set into the OCRnx-register.
		 * \param channel the output channel x which shall be affected by this operation. (upper-case, 'A', 'B',...)
		 * \returns 0 on sucess, >0 if channel was invalid.
		 */
		virtual int8_t setOCRx(uint8_t value, char channel) = 0;

		/*!
		 * Forces an output-compare-match on an specified output-channel when in non-PWM-mode.
		 * This will effect the output of this channel as specified by the Compare-Match-Output-Mode,
		 * but will not trigger any interrupts nor will it clear the timer in CTC-mode!
		 * \param channel the output channel which shall be affected by this operation. (upper-case, 'A', 'B',...)
		 * \returns 0 on sucess, >0 if channel was invalid, <0 if operation is invalid because the timer is currently in PWM-mode.
		 */
		virtual int8_t forceOutputCompare(char channel) = 0;

		/*!
		 * Sets the Compare-Match-Output-Mode value of output-channel x.
		 * Setting this value != 0 will allocate the connected IoPin to block it from other users.
		 * (Freeing the IoPin afterwards is only possible by freeing the entire timer, though access to the IoPin can be gained via "getOutputPin()")
		 * In case the connected IoPin could not be allocated, no value > 0 will be set into the register!
		 * \param value the value to set into the COMnx-bits, range 0b00 - 0b11. (0b00 = no output, check datasheet of µC for other bit-combinations)
		 * \param channel the output channel x which shall be affected by this operation. (upper-case, 'A', 'B',...)
		 * \returns 0 on sucess, <0 if value was invalid, 1 if channel was invalid, >1 if the connected IoPin could not be allocated.
		 */
		virtual int8_t setCOMx(uint8_t value, char channel) = 0;

		/*!
		 * Gets an abstract instance of the IoPin used as output by this timer on a specified channel.
		 * This can be used to manually interact with the output-pins of this timer, which is for example necessary
		 * to enable/disable the actual physical output of this timer by setting the data-direction of the IoPin respectively.
		 * \param channel the output channel which shall be affected by this operation. (upper-case, 'A', 'B',...)
		 * \returns a pointer to the used IoPin on success, nullptr if the channel was invalid or the IoPin could not be allocated.
		 */
		virtual IoPin* getOutputPin(char channel) = 0;

		/*!
		 * Enables the use of the external clock input if supported by this timer.
		 * This will clear the prescalar and start the timer.
		 * To disable the external clock input, either stop the timer or set a prescalar value for this timer.
		 * This method can also be used to test if this timer supports an external clock source.
		 * \param trigger defines the trigger-type to be used by the external clock input. (rising or falling edge)
		 * Enum type ExtClkTrigger is provided for easier usage, else trigger: 6==falling, 7==rising.
		 * \returns 0 on sucess, <0 if trigger had an invalid value, >0 if this timer does not support external clocking.
		 */
		virtual int8_t setExtClockInput(ExtClkTrigger trigger) = 0;

		/*!
		 * Gets an abstract instance of the IoPin used as external clock input by this timer.
		 * If this timer supports asynchronous operation this method will return the Timer-Oscillator1 (TOSC1) IoPin instead.
		 * This method will either allocate the external clock input pin or the asynchronous Timer-Oscillator-Pin1 (TOSC1),
		 * depending on the type of this timer (normal or asynchronous).
		 * Freeing the IoPin(s) afterwards is only possible by freeing the entire timer.
		 * This method can be useful to manually interact with the external clock input, for example to enable software-control of the counting.
		 * \returns a pointer to the used IoPin on success, nullptr if the IoPin could not be allocated or this timer does not support
		 * external or asynchronous clocking.
		 */
		virtual IoPin* getExtClkPin() = 0;

		/*!
		 * Enables/disables the Output-Compare-Match interrupt. This will always clear the corresponding interrupt-flag.
		 * \param value true to enable the interrupt, false to disable the interrupt.
		 * \param channel the Compare-Match channel which shall be affected by this operation. (upper-case, 'A', 'B',...)
		 * Will do nothing if the channel is invalid
		 */
		virtual void enableOutputCompareInterrupt(bool value, char channel) = 0;

		/*!
		 * Enables/disables the Timer-Overflow interrupt. This will always clear the corresponding interrupt-flag.
		 * \param value true to enable the interrupt, false to disable the interrupt.
		 */
		virtual void enableTimerOverflowInterrupt(bool value) = 0;

		/*!
		 * Sets a Listener which gets triggered when a Compare-Match-Interrupt occurs for a given channel. (Will do nothing if the channel is invalid)
		 * Call this method with argument isr==nullptr to remove the isr-listener for this interrupt.
		 * This operation will only have an effect when the corresponding Interrupt-Management by ATmighty is enabled in the Interrupt-Config.h
		 * \param isr the Listener-object which will be triggered to serve as interrupt-service-routine.
		 * \param channel the Compare-Match channel which shall be affected by this operation. (upper-case, 'A', 'B',...)
		 */
		virtual void setOutputCompareISR(Listener* isr, char channel) = 0;

		/*!
		 * Sets a Callback-function which gets called when a Compare-Match-Interrupt occurs for a given channel. (Will do nothing if the channel is invalid)
		 * Call this method with argument isr==nullptr to remove the isr-callback for this interrupt.
		 * This operation will only have an effect when the corresponding Interrupt-Management by ATmighty is enabled in the Interrupt-Config.h
		 * \param isr the functionpointer which will be called to serve as interrupt-service-routine.
		 * \param channel the Compare-Match channel which shall be affected by this operation. (upper-case, 'A', 'B',...)
		 */
		virtual void setOutputCompareISR(void (*isr)(), char channel) = 0;

		/*!
		 * Sets a Listener which gets triggered when a Timer-Overflow-Interrupt occurs.
		 * Call this method with argument isr==nullptr to remove the isr-listener for this interrupt.
		 * This operation will only have an effect when the corresponding Interrupt-Management by ATmighty is enabled in the Interrupt-Config.h
		 * \param isr the Listener-object which will be triggered to serve as interrupt-service-routine.
		 */
		virtual void setTimerOverflowISR(Listener* isr) = 0;

		/*!
		 * Sets a Callback-function which gets called when a Timer-Overflow-Interrupt occurs.
		 * Call this method with argument isr==nullptr to remove the isr-callback for this interrupt.
		 * This operation will only have an effect when the corresponding Interrupt-Management by ATmighty is enabled in the Interrupt-Config.h
		 * \param isr the functionpointer which will be called to serve as interrupt-service-routine.
		 */
		virtual void setTimerOverflowISR(void (*isr)()) = 0;

		/// returns the total number of PWM-output-channels driven by this timer (Channels = A,B,...)
		virtual uint8_t getNumberOfChannels() = 0;

		/// returns the base-clock frequency which drives this timer in Hz. (Will be processor-speed for abstract timers, less for virtual timers)
		virtual uint32_t getBaseFrequency() = 0;

	protected:
		/// returns a string-representation of this type stored in flash
		inline PGM_P getHardwareStringRepresentation()
		{
			static const char me[] PROGMEM = "Timer8bit";
			return me;
		}
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_TIMER_TIMER8BIT_H_ */
