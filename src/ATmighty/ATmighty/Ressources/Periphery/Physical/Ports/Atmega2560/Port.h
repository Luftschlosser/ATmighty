/*!
 * This headerfile is µC-dependent, it should only be included indirectly via "Ports.h".
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_PORTS_ATMEGA2560_PORT_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_PORTS_ATMEGA2560_PORT_H_


#include <stdint.h>
#include "ATmighty/Ressources/Periphery/Utilities/IoPortAddresses.h"
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareBase/PhysicalHardwareBase.h"
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"

/// This class provides access to the PortA-periphery of the atmega2560 µC.
template<char PortChar> class Port : private PhysicalHardwareBase
{
	friend Port<'A'>* PhysicalHardwareManager::Alloc<Port<'A'>>(int8_t id);
	friend int8_t PhysicalHardwareManager::GetOwner<Port<'A'>>();
	friend void PhysicalHardwareManager::Free<Port<'A'>>(Port<'A'> **hardware);

	friend Port<'B'>* PhysicalHardwareManager::Alloc<Port<'B'>>(int8_t id);
	friend int8_t PhysicalHardwareManager::GetOwner<Port<'B'>>();
	friend void PhysicalHardwareManager::Free<Port<'B'>>(Port<'B'> **hardware);

	friend Port<'C'>* PhysicalHardwareManager::Alloc<Port<'C'>>(int8_t id);
	friend int8_t PhysicalHardwareManager::GetOwner<Port<'C'>>();
	friend void PhysicalHardwareManager::Free<Port<'C'>>(Port<'C'> **hardware);

	friend Port<'D'>* PhysicalHardwareManager::Alloc<Port<'D'>>(int8_t id);
	friend int8_t PhysicalHardwareManager::GetOwner<Port<'D'>>();
	friend void PhysicalHardwareManager::Free<Port<'D'>>(Port<'D'> **hardware);

	friend Port<'E'>* PhysicalHardwareManager::Alloc<Port<'E'>>(int8_t id);
	friend int8_t PhysicalHardwareManager::GetOwner<Port<'E'>>();
	friend void PhysicalHardwareManager::Free<Port<'E'>>(Port<'E'> **hardware);

	friend Port<'F'>* PhysicalHardwareManager::Alloc<Port<'F'>>(int8_t id);
	friend int8_t PhysicalHardwareManager::GetOwner<Port<'F'>>();
	friend void PhysicalHardwareManager::Free<Port<'F'>>(Port<'F'> **hardware);

	friend Port<'G'>* PhysicalHardwareManager::Alloc<Port<'G'>>(int8_t id);
	friend int8_t PhysicalHardwareManager::GetOwner<Port<'G'>>();
	friend void PhysicalHardwareManager::Free<Port<'G'>>(Port<'G'> **hardware);

	friend Port<'H'>* PhysicalHardwareManager::Alloc<Port<'H'>>(int8_t id);
	friend int8_t PhysicalHardwareManager::GetOwner<Port<'H'>>();
	friend void PhysicalHardwareManager::Free<Port<'H'>>(Port<'H'> **hardware);

	friend Port<'J'>* PhysicalHardwareManager::Alloc<Port<'J'>>(int8_t id);
	friend int8_t PhysicalHardwareManager::GetOwner<Port<'J'>>();
	friend void PhysicalHardwareManager::Free<Port<'J'>>(Port<'J'> **hardware);

	friend Port<'K'>* PhysicalHardwareManager::Alloc<Port<'K'>>(int8_t id);
	friend int8_t PhysicalHardwareManager::GetOwner<Port<'K'>>();
	friend void PhysicalHardwareManager::Free<Port<'K'>>(Port<'K'> **hardware);

	friend Port<'L'>* PhysicalHardwareManager::Alloc<Port<'L'>>(int8_t id);
	friend int8_t PhysicalHardwareManager::GetOwner<Port<'L'>>();
	friend void PhysicalHardwareManager::Free<Port<'L'>>(Port<'L'> **hardware);

	private:
		/// Default Constructor
		inline Port() : PhysicalHardwareBase() {}

		/// returns a singleton-instance
		inline static Port<PortChar>& GetInstance()
		{
			static Port<PortChar> instance;
			return instance;
		}

		/// returns a string-representation of this type stored in flash
		static const char* GetHardwareStringRepresentation();

	public:
		///Sets the value of the PORTA register
		inline void setPORTx(uint8_t value)
		{
			*(IoPortAddresses::CharToAddress(PortChar) + 2) = value; //compiler will optimize to single instruction
		}

		///Sets the value of the DDRA register
		inline void setDDRx(uint8_t value)
		{
			*(IoPortAddresses::CharToAddress(PortChar) + 1) = value; //compiler will optimize to single instruction
		}

		///Sets the value of the PINA register
		inline void setPINx(uint8_t value)
		{
			*(IoPortAddresses::CharToAddress(PortChar)) = value; //compiler will optimize to single instruction
		}

		///Returns the current value of the PORTA register
		inline uint8_t getPORTx()
		{
			return *(IoPortAddresses::CharToAddress(PortChar) + 2); //compiler will optimize to single instruction
		}

		///Returns the current value of the DDRA register
		inline uint8_t getDDRx()
		{
			return *(IoPortAddresses::CharToAddress(PortChar) + 1); //compiler will optimize to single instruction
		}

		///Returns the current value of the PINA register
		inline uint8_t getPINx()
		{
			return *(IoPortAddresses::CharToAddress(PortChar)); //compiler will optimize to single instruction
		}
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_PORTS_ATMEGA2560_PORT_H_ */
