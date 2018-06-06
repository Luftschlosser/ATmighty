/*!
 * This headerfile represents an IoPin which can wrap any IoPin, configurable by constructor-parameters
 * This headerfile is µC-dependent, it should only be included indirectly via "IoPins.h".
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPINS_GENERALIOPIN_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPINS_GENERALIOPIN_H_


#include <ATmighty/Ressources/Periphery/Abstract/IoPins/AbstractIoPin.h>


/// This class abstractly wraps any specified physical IoPin. As the exact pin is unknown at compile-time, this implementation is relatively slow.
class GeneralIoPin : public AbstractIoPin
{
	friend class AbstractHardwareManager;

	private:
		///The owner-id who owns this abstract hardware instance
		int8_t owner;

		///The number of the pin to wrap (0-7)
		uint8_t pin;

		///The address off the "PINX" -register of the used port. "DDRX" is located at portAddress+1, "PORTX" is located at portAddress+2.
		volatile uint8_t* portAddress;

		/*!
		 * Constructor
		 * \param portChar The character-symbol of the port the desired pin is part of in upper case ('A'-'L')
		 * \param pinNumber The number of the desired pin within the port (0-7)
		 */
		GeneralIoPin(char portChar, uint8_t pinNumber);

		///Destructor
		~GeneralIoPin() {}

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
		inline int8_t getOwner() {return owner;}

	public:
		/// Sets the data-direction of this pin. ('0' = in, '>0' = out)
		void setDirection(DataDirection direction);

		/*!
		 * Sets the output logic-level of this pin, when configured as output-pin. (true = high, false = low)
		 * Enables/disables the pullup-resistor of this pin, when configured as input-pin (true = enable, false = disable)
		 */
		void set(bool value);

		/// toggles the output of this pin independently of the data-direction
		void toggle();

		/// returns the current data-direction of this pin
		DataDirection getDirection();

		/// returns the current value of this pin, independently of the data-direction. (true = high, false = low)
		bool read();

		/// Returns the corresponding port-character associated with the specific port used by this IoPin abstraction \returns ('A' - 'L')
		char getPinPort();

		/// Returns the corresponding pin-number associated with this specific pin within its IoPort (as printable character) \returns ('0' - '7')
		inline char getPinNumber() {return '0'+pin;}
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPINS_GENERALIOPIN_H_ */
