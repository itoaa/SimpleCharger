/*
 * sGPIO.cpp
 *
 *  Created on: 24 feb. 2020
 *      Author: Ola Andersson
 */

#include "sEEPROM.hpp"

sEEPROM::sEEPROM()										// defaukr constructor. Dose nothing
{
}

sEEPROM::sEEPROM(uint8_t mode)						// may be implemanted later
{
};

void sEEPROM::init()
{
};

uint8_t	sEEPROM::readByte(uint16_t address)
{
	uint8_t st = eeprom_read_byte((const uint8_t*)address);
return(st);
}

void	sEEPROM::writeByte(uint16_t address, uint8_t data)
{
	eeprom_write_byte((uint8_t*)address, (uint8_t)data);
}

void	sEEPROM::eraseEEPROM()
{
}
