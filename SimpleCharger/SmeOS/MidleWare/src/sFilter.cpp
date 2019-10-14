/*
 * sFilter.cpp
 *
 *  Created on: 10 dec. 2018
 *      Author: ola
 */

#include "sFilter.hpp"

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

Filter::Filter(uint8_t bufferL)
{
	pBufferLength = bufferL;
	pBufferPosition = 0;
}

/*
Filter::~Filter()
{
	// TODO Auto-generated destructor stub
}
*/

// Add one value to the buffer.
void Filter::addValue(int16_t value)
{
	int j;
	int16_t temp;
	bool done;

	pBufferPosition = (pBufferPosition + 1) % filterSamples;    // increment counter and roll over if necc. -  % (modulo operator) rolls over variable
	pSensArray1[pBufferPosition] = value;                       // input new data into new slot

	for (j=0; j<filterSamples; j++)
	{                                                           // transfer data array into anther array for sorting and averaging
		pSensArray1Sort[j] = pSensArray1[j];
	}
	done = 0;                                                   // flag to know when we're done sorting
	while(done != 1)
	{                                                           // simple swap sort, sorts numbers from lowest to highest
		done = 1;
		for (j = 0; j < (filterSamples -1); j++)
		{
			if (pSensArray1Sort[j] > pSensArray1Sort[j + 1])    // numbers are out of order - swap
			{
				temp = pSensArray1Sort[j + 1];
				pSensArray1Sort [j+1] =  pSensArray1Sort[j] ;
				pSensArray1Sort [j] = temp;
				done = 0;
			}
		}
	}
}
                                                                       	   // Get filtered and averaged value based on buffer values.
int16_t Filter::getFilteredValue(void)
{
	int16_t  top, bottom, k, l;
	long total;
                                                                       	   // throw out top and bottom 15% of samples - limit to throw out at least one from top and bottom
	bottom = max(((filterSamples * 15)  / 100), 1);
	top = min((((filterSamples * 85) / 100) + 1  ), (filterSamples - 1));  // the + 1 is to make up for asymmetry caused by integer rounding
	k = 0;
	total = 0;
	for ( l = bottom; l< top; l++)
	{
		total += pSensArray1Sort[k];                                       // total remaining indices
		k++;
	}
	return( total / k);                                                    // divide by number of samples
}


int16_t Filter::getValue(uint8_t index)                                     // Get valu from buffer. index = position in buffer. (0 = newest value)
{
	return (pSensArray1[index]);
}
int16_t Filter::getSortValue(uint8_t index)                                 // Get value from sorted buffer. index = position in sorted buffer. (0 = lowes value)
{
	return (pSensArray1Sort[index]);
}



