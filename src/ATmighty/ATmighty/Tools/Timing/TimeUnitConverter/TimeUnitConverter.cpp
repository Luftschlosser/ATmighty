/*!
 * Implements \see TimeUnitConverter (only Constructor, rest will be inlined).
 * This is only defined in external file to explizitely instantiate the class template with senseful types
 */


#include "TimeUnitConverter.h"


template <typename CycleInt_T> TimeUnitConverter<CycleInt_T>::TimeUnitConverter(uint32_t baseFrequency) : baseFrequency(baseFrequency)
{
}


//Explizit template instantiations
template class TimeUnitConverter<uint16_t>;
template class TimeUnitConverter<uint32_t>;
template class TimeUnitConverter<uint64_t>;
