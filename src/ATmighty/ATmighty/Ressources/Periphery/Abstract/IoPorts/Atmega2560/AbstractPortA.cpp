/*!
 * This file implements the class \see AbstractPortA
 */


#include "AbstractPortA.h"
#include "ATmighty/Utilities/LUTs/HardwareOwnerID.h"


int8_t AbstractPortA::init(int8_t ownerId)
{
	if (AbstractHardwareBase::init(ownerId))
	{
		physicalReference = PhysicalHardwareManager::Alloc<PortA>(OwnerID::DirectAbstraction);
		if (physicalReference != nullptr)
		{
			return 0;
		}
		else
		{
			AbstractHardwareBase::exit();
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
	AbstractHardwareBase::exit();
	PhysicalHardwareManager::Free<PortA>(&physicalReference);
}
