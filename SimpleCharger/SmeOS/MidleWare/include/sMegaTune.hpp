/*
 * sMegaTuneSimp10.hpp
 *
 *  Created on: 28 dec. 2018
 *      Author: ola
 */

#ifndef SMEOS_MIDLEWARE_INCLUDE_SMEGATUNE_HPP_
#define SMEOS_MIDLEWARE_INCLUDE_SMEGATUNE_HPP_

#include <stdint.h>
#include <FreeRTOS.h>
#include <avr/io.h>
#include <task.h>
#include "sSerial.hpp"
#include "sDB.hpp"

#define CodeString	"MSII s-Charger B-1  "       	// String sent to TuningStudio, use F() macro to store in progmem, not in sram.
#define CodeRev		"Smedby simple Charger Beta 1"	// Code Revision to send to Tunerstudio. Title Bar in tunerStudio

// MegaTune serial communication class
class sMegaTune
{
private:
	int	sec;
	sSerial MySerial;

public:
	sMegaTune(int);							// Setup serial port speed for MegaTune communication.
	void send_Sec(int);						// Send number of seconds passed
	void send_Rev(char*);					// Send code revision.
	void send_EpromVar();					// Send EEprom variables.
	void get_EpromVar();					// Get EEprom variables.
	void load_EpromVars();					// Load pg1-vars from EEPROM, only done at startup.
	void send_RPage();						// Send real time data

	void processSerial();
	bool dataRecived();

};

#endif /* SMEOS_MIDLEWARE_INCLUDE_SMEGATUNE_HPP_ */
