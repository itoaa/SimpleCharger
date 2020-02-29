/*
 * sDB.cpp
 *
 *  Created on: 29 Feb 2020
 *      Author: olaandersson
 */

#include <sDB.hpp>
sDB GlobalDB;

sDB::sDB() {
	// TODO Auto-generated constructor stub
	variab = 0;
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
	temppointer = (uint8_t*)&pg1 + offset;
	*temppointer = data;
}
