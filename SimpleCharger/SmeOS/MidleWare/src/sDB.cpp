/*
 * sDB.cpp
 *
 *  Created on: 29 Feb 2020
 *      Author: olaandersson
 */

#include <sDB.hpp>
sDB GlobalDB;

sDB::sDB() {
	    loadPG1();
}

uint8_t sDB::getpg1Data(uint8_t offset)
{
	uint8_t* temppointer;
	temppointer = (uint8_t*)&pg1 + offset;

	return(*temppointer);
}
void sDB::setpg1Data(uint8_t offset, uint8_t data)
{
	uint8_t* temppointer;
	temppointer = (uint8_t*)&rtPage + offset;
	*temppointer = data;
}

uint8_t sDB::getrtpData(uint8_t offset)

{
	uint8_t* temppointer;
	temppointer = (uint8_t*)&rtPage + offset;

	return(*temppointer);
}
void sDB::setrtpData(uint8_t offset, uint8_t data)
{
	uint8_t* temppointer;
	temppointer = (uint8_t*)&pg1 + offset;
	*temppointer = data;
}

void sDB::savePG1()
// Save pg1 to EEPROM
{
	int i;
	char *temppointer;
//	vTaskSuspendAll();									// Maybe not needed
	for (i=0; i < PG1S; i++)     						// i = 0 till page1size
	{
		temppointer = (char*)&pg1 + i;					// temppointer pekar på nästa position i pg1pointer
		MyEEPROM.writeByte(i+PG1Offset,*temppointer);	// Write specifyed byte from struct in to EEPROM
	}
//	xTaskResumeAll();
}
void sDB::loadPG1()
// Load pg1 from EEPROM
{
	int i;
	char *temppointer;
//	vTaskSuspendAll();									// Maybe not needed
	for (i=0; i < PG1S; i++)     						// i = 0 till page1size
	{
		temppointer = (char*)&pg1 + i;					// temppointer pekar på nästa position i pg1 pointer
		*temppointer = MyEEPROM.readByte(i+PG1Offset); 	// read value from EEPROM directly into struct.
	}
//	xTaskResumeAll();
}
