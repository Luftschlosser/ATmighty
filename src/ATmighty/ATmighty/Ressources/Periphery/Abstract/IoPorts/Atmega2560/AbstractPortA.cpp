/*!
 * This file implements the class \see AbstractPortA
 */


#include "AbstractPortA.h"
#include "ATmighty/Utilities/LUTs/HardwareOwnerID.h"
#include "ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h"


int8_t AbstractPortA::init(int8_t ownerId)
{
	if (!AbstractIoPort::init(ownerId))
	{
		physicalReference = PhysicalHardwareManager::Alloc<PortA>(OwnerID::DirectAbstraction);
		if (physicalReference != nullptr)
		{
			return 0;
		}
		else
		{
			AbstractIoPort::exit();
			return -1;
		}
	}
	else
	{
		return 1;
	}
}

void AbstractPortA::exit()
{
	PhysicalHardwareManager::Free(&physicalReference);
	AbstractIoPort::exit();
}
