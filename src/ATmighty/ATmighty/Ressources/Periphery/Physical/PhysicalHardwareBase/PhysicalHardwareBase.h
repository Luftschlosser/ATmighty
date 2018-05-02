/*!
 * This headerfile is µC-independent. It declares class PhysicalHardwareBase, which is the base class for all physical hardware.
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_PHYSICALHARDWAREBASE_PHYSICALHARDWAREBASE_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_PHYSICALHARDWAREBASE_PHYSICALHARDWAREBASE_H_


#include <stdbool.h>
#include <stdint.h>

/// This class is a common baseclass for all physical hardware and manages the exclusive access of each hardware.
class PhysicalHardwareBase
{
	private:
		/// 1 Byte overhead stores owner (0 if free) - Positive range is for user, Negative range is for ATmighty
		volatile int8_t owner;

	public:
		/// Constructor (initializes this physical hardware as "free")
		inline PhysicalHardwareBase() : owner(0) {}

		/// Default Destructor
		inline ~PhysicalHardwareBase() {}

		/// Checks if this physical hardware is in use
		inline bool isFree() {return (owner == 0);}

		/// Returns the current owner id
		inline int8_t getOwner() {return owner;}

		/// frees this physical hardware by setting the owner to 0
		inline void free() {owner = 0;}

		/// allocates this physical hardware by setting the owner != 0. WARNING: This may override current owner!
		inline void allocate(int8_t owner) {this->owner = owner;}
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_PHYSICAL_PHYSICALHARDWAREBASE_PHYSICALHARDWAREBASE_H_ */
