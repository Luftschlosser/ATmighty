/*!
 * This headerfile is µC-dependent, it should only be included indirectly via "IoPins.h".
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPINS_ATMEGA2560_ABSTRACTPINA0_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPINS_ATMEGA2560_ABSTRACTPINA0_H_


#include <ATmighty/Ressources/Periphery/Abstract/IoPins/AbstractIoPin.h>


/// This class abstractly wraps physical pin A0 at PortA
class AbstractPinA0 : public AbstractIoPin
{
	friend class AbstractHardwareManager;

	private:
		///Default Constructor
		inline AbstractPinA0() {}

		///Destructor
		~AbstractPinA0() {}

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

		/*!
		 * Returns a singleton-instance. Each specific abstract hardware is a singleton, as the same physical hardware can only be abstracted once.
		 * This implementation was chosen because it does not require the use of new/delete.
		 */
		static inline AbstractPinA0& GetInstance()
		{
			static AbstractPinA0 instance;
			return instance;
		}

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

		/// Returns the corresponding letter (upper-case) associated with the specific physical IoPort used by this AbstractIoPin \returns ('A'-'L')
		inline char getCharCode() {return 'A';}

		/// Returns the corresponding pin-number associated with this specific pin within its IoPort (as printable character) \returns ('0' - '7')
		inline char getPinNumber() {return '0';}
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPINS_ATMEGA2560_ABSTRACTPINA0_H_ */
