/*!
 * This headerfile is µC-dependent, it should only be included indirectly via "IoPorts.h".
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPORTS_ATMEGA2560_ABSTRACTPORTA_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPORTS_ATMEGA2560_ABSTRACTPORTA_H_


#include "ATmighty/Ressources/Periphery/Abstract/IoPorts/IoPort.h"
#include "ATmighty/Ressources/Periphery/Physical/IoPorts.h"
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"


class AbstractPortA : public IoPort
{
	friend class AbstractHardwareManager;

	private:
		PortA* physicalReference;

		inline AbstractPortA() : physicalReference(nullptr) {}

		virtual ~AbstractPortA() {}

		int8_t init(int8_t ownerId);

		void exit();

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
		inline char getPortLetter() {return 'A';}
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_ABSTRACT_IOPORTS_ATMEGA2560_ABSTRACTPORTA_H_ */
