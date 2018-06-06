/*!
 * This headerfile represents an IoPort which can wrap any IoPort, configurable by constructor-parameters
 * This headerfile is µC-dependent, it should only be included indirectly via "IoPorts.h".
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPORTS_ATMEGA2560_GENERALIOPORT_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPORTS_ATMEGA2560_GENERALIOPORT_H_


#include <ATmighty/Ressources/Periphery/Abstract/IoPorts/AbstractIoPort.h>


/// This class abstractly wraps any specified physical IoPort. As the exact port is unknown at compile-time, this implementation is relatively slow.
class GeneralIoPort : AbstractIoPort
{
	friend class AbstractHardwareManager;

	private:
		///The owner-id who owns this abstract hardware instance
		int8_t owner;

		///A reference to the physical IoPort-instance wrapped by this GeneralIoPort
		void* physicalReference;

		///The address off the "PINX" -register of the wrapped port. "DDRX" is located at portAddress+1, "PORTX" is located at portAddress+2.
		volatile uint8_t* portAddress;

		/*!
		 * Constructor
		 * \param portChar The character-symbol of the port to wrap in upper case ('A'-'L')
		 */
		GeneralIoPort(char portChar);

		///Destructor
		~GeneralIoPort() {}

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
		/// Sets the data-direction-bits of this Port. ('1' = out, '0' = in)
		void setDataDirectionMask(uint8_t mask);

		/*!
		 * Sets the data-register of this Port.
		 * Pins which are configured as output: Sets the output logic-level ('1' = high, '0' = low)
		 * Pins which are configured as input: Sets the pullup-resistors ('1' = enable, '0' = disable)
		 */
		void setData(uint8_t data);

		/// toggles the Pins where mask == '1' independently of the data-direction.
		void applyPinToggleMask(uint8_t mask);

		/// Reads the data-direction-bits of this Port. ('1' = out, '0' = in)
		uint8_t getDataDirections();

		/// Reads the value of the pins of this Port independently of the data-direction.
		uint8_t getPinValues();

		/// Returns the corresponding letter (upper-case) associated with this IoPort.
		char getCharCode();
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPORTS_ATMEGA2560_GENERALIOPORT_H_ */
