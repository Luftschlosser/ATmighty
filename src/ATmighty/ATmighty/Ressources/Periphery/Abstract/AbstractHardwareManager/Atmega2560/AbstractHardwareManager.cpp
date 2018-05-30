/*!
 * This file implements the class \see AbstractHardwareManager
 */

#include "AbstractHardwareManager.h"

#include <util/atomic.h>
#include "ATmighty/Ressources/Periphery/Abstract/AbstractHardwareBase/AbstractHardwareBase.h"
#include "ATmighty/Ressources/Periphery/Abstract/IoPorts.h"
#include "ATmighty/Utilities/Logs/MessageLog.h"


template<class Hw> Hw* AbstractHardwareManager::Alloc (int8_t id)
{
	static Hw ref;
	Hw* ret;

	if (id == 0) {id = -1;} //validate ownerID

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{	if (ref.init(id) == 0)
		{
			ret = &ref;
		}
		else
		{
			ret = nullptr;
		}
	}

	return ret;
}

IoPort* AbstractHardwareManager::AllocIoPort(int8_t id)
{
	return (IoPort*) Alloc<AbstractPortA>(id);
}

void AbstractHardwareManager::Free(AbstractHardwareBase **hardware)
{
	if (hardware != nullptr && (*hardware) != nullptr)
	{
		(*hardware)->exit();
		(*hardware) = nullptr;
	}
}


//The explizitely instantiated versions of the template-functions (update the list when adding new types!)
template AbstractPortA* AbstractHardwareManager::Alloc<AbstractPortA>(int8_t id);
