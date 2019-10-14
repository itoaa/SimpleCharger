/*
 * sFilter.hpp
 *
 *  Created on: 10 dec. 2018
 *      Author: ola
 */

#ifndef SMEOS_MIDLEWARE_INCLUDE_SFILTER_HPP_
#define SMEOS_MIDLEWARE_INCLUDE_SFILTER_HPP_

#include "stdio.h"

#define filterSamples   13                // filterSamples should  be an odd number, no smaller than 3

class Filter {
	private:
		int16_t pSensArray1 [filterSamples];        // array for holding raw sensor values for sensor1
		int16_t pSensArray1Sort [filterSamples];    // array for holding sorted sensor values for sensor1
		uint8_t pBufferPosition;
		uint8_t	pBufferLength;
	public:
		Filter(uint8_t bufferLength);			// BuffesLength = number of samples in buffer.
//		virtual ~Filter();
		int16_t getValue(uint8_t index);               // Get valu from buffer. index = position in buffer. (0 = newest value)
		int16_t getSortValue(uint8_t index);           // Get value from sorted buffer. index = position in sorted buffer. (0 = lowes value)
		int16_t getFilteredValue(void);             // Get filtered and averaged value based on buffer values.
		void addValue(int16_t value);               // Add one valu to the buffer.
};


#endif /* SMEOS_MIDLEWARE_INCLUDE_SFILTER_HPP_ */
