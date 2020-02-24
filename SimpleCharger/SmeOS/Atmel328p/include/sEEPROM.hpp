/*
 * sEEPROM.hpp
 *
 *  Created on: 24 feb. 2020
 *      Author: Ola Andersson
 */

#include "stdint.h"
#include "avr/io.h"
#include "avr/eeprom.h"


#ifndef SMEOS_ATMEL328P_INCLUDE_SEEPROM_HPP_
#define SMEOS_ATMEL328P_INCLUDE_SEEPROM_HPP_


class sEEPROM
{
  private:

  public:
	sEEPROM(uint8_t mode);									// may use mode in the future to read other than internal eeprom
	sEEPROM();												// Init internal eeprom

	void init();											// use if using default constructor
	uint8_t	readByte(uint16_t address);						// Read one byte from address
	void	writeByte(uint16_t address, uint8_t data);		// Write one byte from address
	void	eraseEEPROM();									// Erase eeprom

 };

#endif /* SMEOS_ATMEL328P_INCLUDE_SEEPROM_HPP_ */
