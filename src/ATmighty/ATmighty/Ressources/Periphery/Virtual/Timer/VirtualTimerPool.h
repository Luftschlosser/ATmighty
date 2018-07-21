/*
 * This headerfile is µC-independent.
 */

#ifndef ATMIGHTY_RESSOURCES_PERIPHERY_VIRTUAL_TIMER_VIRTUALTIMERPOOL_H_
#define ATMIGHTY_RESSOURCES_PERIPHERY_VIRTUAL_TIMER_VIRTUALTIMERPOOL_H_


#include "ATmighty/Interfaces/Listener.h"
#include "ATmighty/Tools/Timing/PeriodicTrigger/PeriodicTrigger.h"
#include "ATmighty/Ressources/Periphery/Virtual/Timer/VirtualTimer8bit.h"


/*!
 * A VirtualTimerPool uses one Timer to generate 1-8 virtual Timers which can be used like real abstract timers for most purposes.
 */
template<class Timer = Timer16bit> class VirtualTimerPool : private Listener
{
	private:
		//type-defintion for a union which can either hold an 8bit or an 16bit virtual timer.
		typedef union virtualTimerUnion
		{
			VirtualTimer8bit *t8b; //TODO: Change implementation to not use as much dynamically allocated memory!
			//VirtualTimer16bit t16b; //Todo: add virtual 16-bit timers
		} vtimer;

		//the trigger source which is used to initiate a counter step
		PeriodicTrigger<Timer> clock;

		//The frequency at which the virtual timers run.
		uint16_t triggerFrequency;

		//an array of virtual timers
		vtimer* vtimers;

		//bits map to indices in the vtimers-array: [0 = vtimer at given index in use / 1 = given index free for use]
		uint8_t usageMap;

		//bits map to indices in the vtimers-array: [0 = vtimer at given index is 8bit-timer / 1 = vtimer at given index is 16bit-timer]
		uint8_t typeMap;

		//the maximal number of virtual timers in this VirtualTimerPool
		uint8_t poolsize;

		//this will trigger a timerStep for the virtual timers. It gets called internally by a PeriodicTrigger-Instance.
		void trigger();

	public:
		/*!
		 * Constructor
		 * \param baseFrequency the base frequency the virtualized timers shall have.
		 * (All virtual Timers from one VirtualTimerPool will have the same base-frequency)
		 * \param sourceTimer A pointer to a timer which is used to create a clock for the virtual timers
		 * \param poolsize The number of virtual timers that can be created from this pool. [1 - 8] (more than 8 would create a massive overhead)
		 * If you absolutely need more than 8 virtual Timers from one sourceTimer you can achieve that by nesting multiple VirtualTimerPools, but do this
		 * only when your virtual timers run at very low frequencies!
		 */
		VirtualTimerPool(uint16_t baseFrequency, Timer* sourceTimer, uint8_t poolsize);

		///Destructor
		~VirtualTimerPool();

		/*!
		 * Generates a new virtual 8bit-Timer without physical outputs.
		 * \param channels [optional, default 1] The number of output-compare-channels of the newly generated virtual 8bit-Timer. [range 1-4]
		 * \returns A pointer to the newly generated virtual 8bit-Timer or nullptr if this VirtualTimerPool is fully used and cannot virtualize another timer.
		 */
		VirtualTimer8bit* allocTimer8bit(uint8_t channels = 1);

		/*!
		 * Generates a new virtual 8bit-Timer with physical outputs.
		 * \param channels The number of output-compare-channels of the newly generated virtual 8bit-Timer. [range 1-4]
		 * \param outputPins An array of IoPins with size==channels to function as physical Timer-outputs.
		 * \returns A pointer to the newly generated virtual 8bit-Timer or nullptr if the virtual timer could not be created because
		 * either this VirtualTimerPool is fully used and cannot virtualize another timer or if outputPins was nullptr.
		 */
		VirtualTimer8bit* allocTimer8bit(uint8_t channels, IoPin** outputPins);

		/*!
		 * Destroys an virtual 8bit-timer and frees its place within this virtualTimerPool to be used for other virtual Timers.
		 * This leaves the caller with a nullptr-reference to the original virtual timer.
		 * \param timer A pointer to the pointer to the virtual 8bit-timer to destroy. (Double indirection is needed to prohibit the user from
		 * destroying the same virtual 8bit-timer twice).
		 */
		void freeTimer(VirtualTimer8bit** timer);

		///Starts all virtualized timers. This is needed in order for them to work.
		inline void startAll() {clock.start();}

		///Stops all virtualized timers.
		inline void stopAll() {clock.stop();}
};


#endif /* ATMIGHTY_RESSOURCES_PERIPHERY_VIRTUAL_TIMER_VIRTUALTIMERPOOL_H_ */
