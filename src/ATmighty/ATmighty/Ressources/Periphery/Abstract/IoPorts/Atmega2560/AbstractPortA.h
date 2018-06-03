/*!
 * This headerfile is µC-dependent, it should only be included indirectly via "IoPorts.h".
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPORTS_ATMEGA2560_ABSTRACTPORTA_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPORTS_ATMEGA2560_ABSTRACTPORTA_H_


#include <ATmighty/Ressources/Periphery/Abstract/IoPorts/AbstractIoPort.h>
#include "ATmighty/Ressources/Periphery/Physical/IoPorts.h"
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"


class AbstractPortA : public AbstractIoPort
{
	friend class AbstractHardwareManager;

	private:
		///A pointer to the physical PortA-wrapper to be used by this abstraction
		PortA* physicalReference;

		///Default Constructor
		inline AbstractPortA() : physicalReference(nullptr) {}

		///Destructor
		~AbstractPortA() {}

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
		static inline AbstractPortA& GetInstance()
		{
			static AbstractPortA instance;
			return instance;
		}

	public:
		/// Sets the data-direction-bits of Port A. ('1' = out, '0' = in)
		inline void setDataDirectionMask(uint8_t mask) {physicalReference->setDDRA(mask);}

		/*!
		 * Sets the data-register of Port A.
		 * Pins which are configured as output: Sets the output logic-level ('1' = high, '0' = low)
		 * Pins which are configured as input: Sets the pullup-resistors ('1' = enable, '0' = disable)
		 */
		inline void setData(uint8_t data) {physicalReference->setPORTA(data);}

		/// toggles the Pins where mask == '1' independently of the data-direction.
		inline void applyPinToggleMask(uint8_t mask) {physicalReference->setPINA(mask);};

		/// Reads the data-direction-bits of PortA. ('1' = out, '0' = in)
		inline uint8_t getDataDirections() {return physicalReference->getDDRA();}

		/// Reads the value of the pins of Port A independently of the data-direction.
		inline uint8_t getPinValues() {return physicalReference->getPINA();}

		/// Returns the corresponding letter (upper-case) associated with Port A.
		inline char getCharCode() {return 'A';}
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPORTS_ATMEGA2560_ABSTRACTPORTA_H_ */
