/*
 * This headerfile is µC independent
 */

#ifndef ATMIGHTY_TOOLS_TIMING_TIMEUNITCONVERTER_TIMEUNITCONVERTER_H_
#define ATMIGHTY_TOOLS_TIMING_TIMEUNITCONVERTER_TIMEUNITCONVERTER_H_


#include <stdint.h>


/*!
 * This class provides conversion-methods between SI-Timeunits (seconds, milliseconds, microseconds, hertz) and CPU-Cycles.
 * The template parameter is the uint-type in which CPU-cycles are calculated (uint16_t, uin32_t or uint64_t).

 */
template<typename CycleInt_T = uint32_t> class TimeUnitConverter
{
	private:
		///The base Frequency (CPU-Cycles per second) to use for the calculations.
		uint32_t baseFrequency;

	public:
		/*!
		 * Constructor
		 * \param baseFrequency The number of CPU-Cycles per second to use for the calculations. (must be > 0)
		 */
		TimeUnitConverter(uint32_t baseFrequency);

		inline uint32_t getBaseFrequency() { return baseFrequency; }

		inline CycleInt_T secondsToCycles(uint16_t seconds) { return (CycleInt_T)(baseFrequency * (CycleInt_T)seconds); }
		inline CycleInt_T millisecondsToCycles(uint32_t milliseconds) { return (CycleInt_T)(((uint64_t)baseFrequency * milliseconds) / 1000); } //need temporary 64-bit for high precision
		inline CycleInt_T microsecondsToCycles(uint32_t microseconds) { return (CycleInt_T)(((uint64_t)baseFrequency * microseconds) / 1000000); } //need temporary 64-bit for high precision
		inline CycleInt_T hertzToCycles(uint32_t hertz) { return (CycleInt_T)(baseFrequency / hertz); }

		inline uint16_t cyclesToSeconds(CycleInt_T cycles) { return (uint16_t)(cycles / baseFrequency); }
		inline uint32_t cyclesToMilliseconds(CycleInt_T cycles) { return (uint32_t)(((uint64_t)cycles * 1000) / baseFrequency); } //need temporary 64-bit for high precision
		inline uint32_t cyclesToMicroseconds(CycleInt_T cycles) { return (uint32_t)(((uint64_t)cycles * 1000000) / baseFrequency); } //need temporary 64-bit for high precision
		inline uint32_t cyclesToHertz(CycleInt_T cycles) { return (uint32_t)(baseFrequency / cycles); }
};


#endif /* ATMIGHTY_TOOLS_TIMING_TIMEUNITCONVERTER_TIMEUNITCONVERTER_H_ */
