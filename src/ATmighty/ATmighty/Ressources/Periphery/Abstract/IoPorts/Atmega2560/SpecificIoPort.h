/*!
 * This headerfile represents an IoPort which can wrap any IoPort, configurable by template-parameters
 * This headerfile is µC-dependent, it should only be included indirectly via "IoPorts.h".
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPORTS_ATMEGA2560_SPECIFICIOPORT_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPORTS_ATMEGA2560_SPECIFICIOPORT_H_


#include <ATmighty/Ressources/Periphery/Abstract/IoPorts/AbstractIoPort.h>


/*!
 * This class abstractly wraps any specified physical IoPort. As the exact port is known at compile-time, this implementation is extremely fast.
 * The template parameter "PortChar" can be any upper-case character which specifies the IoPort.
 */
template<char PortChar> class SpecificIoPort : public AbstractIoPort
{
	friend class AbstractHardwareManager;

	private:
		///static owner-id, as a specific IoPort should behave like a Singleton and can only be owned once.
		static int8_t Owner;

		///A reference to the physical IoPort-instance wrapped by this SpecificIoPort
		void* physicalReference;

		///Default Constructor
		inline SpecificIoPort() : physicalReference(nullptr) {}

		///Destructor
		~SpecificIoPort() {}

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
		inline char getCharCode() {return PortChar;}
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPORTS_ATMEGA2560_SPECIFICIOPORT_H_ */
