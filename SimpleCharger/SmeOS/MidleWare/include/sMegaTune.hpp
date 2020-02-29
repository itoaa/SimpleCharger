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
#include "sEEPROM.hpp"


// Define user typed input stored in EEPROM.
// It is only one large strukt of data
struct __attribute__((packed,aligned(1))) Page1DataStruct {
    uint8_t ChargeAmp;
    uint8_t MaxChargeAmp;
    uint8_t MaxChargeVolt;
    uint8_t MaxInputAmp;
    uint8_t MaxInputVolt;
    uint8_t MaxPWM;
    uint8_t MinPWM;
    uint8_t MaxTemp;
    uint8_t accDelay;

    uint8_t mosfetTempADCTable[12];
    int8_t  mosfetTempTable[12];

   uint8_t	s1ChargMetod;
   uint8_t	s2ChargMetod;
   uint8_t	s3ChargMetod;
   uint8_t	s4ChargMetod;

   uint8_t	s1ChargeCurrent;
   uint8_t	s1ChargeVolt;
   int8_t	s1MaxTemp;
   int8_t	s1MinTemp;
   uint8_t	s1MaxVolt;
   uint8_t	s1MinVolt;
   uint8_t	s1MaxCurrent;
   uint8_t	s1MinCurrent;
   uint16_t	s1MaxTime;
   uint8_t	s1MinTime;

   uint8_t	s2ChargeCurrent;
   uint8_t	s2ChargeVolt;
   int8_t	s2MaxTemp;
   int8_t	s2MinTemp;
   uint8_t	s2MaxVolt;
   uint8_t	s2MinVolt;
   uint8_t	s2MaxCurrent;
   uint8_t	s2MinCurrent;
   uint16_t	s2MaxTime;
   uint8_t	s2MinTime;

   uint8_t	s3ChargeCurrent;
   uint8_t	s3ChargeVolt;
   int8_t	s3MaxTemp;
   int8_t	s3MinTemp;
   uint8_t	s3MaxVolt;
   uint8_t	s3MinVolt;
   uint8_t	s3MaxCurrent;
   uint8_t	s3MinCurrent;
   uint16_t	s3MaxTime;
   uint8_t	s3MinTime;

   uint8_t	s4ChargeCurrent;
   uint8_t	s4ChargeVolt;
   int8_t	s4MaxTemp;
   int8_t	s4MinTemp;
   uint8_t	s4MaxVolt;
   uint8_t	s4MinVolt;
   uint8_t	s4MaxCurrent;
   uint8_t	s4MinCurrent;
   uint16_t	s4MaxTime;
   uint8_t	s4MinTime;


};
// The structure of the realtime data sent on the Serial port to megatune
struct __attribute__((packed,aligned(1))) RPageVarsStruct
{
    uint8_t		seconds;					// Seconds
    uint8_t		pw1;						// Pulse with (PWM) in %
    uint8_t		freq;						// pwm out frequensy / 1000 (khz)
    int8_t		batteryTemp;				// Battery temperatur (-128-128)
    int8_t		InputAmp;					// input Current
    int8_t		InputVolt;					// Input Volt
    int8_t		OutputAmp;					// Output Current
    int8_t    	OutputVolt;					// Output Volt
    int8_t    	mosfetTemp;					// MosFet teperatur (-128 to 128 grader C).
    int8_t    	extraTemp;					// Extra temperatur sensor.
    uint8_t    	mosfetDriverVolt;			// Mosfet driver volt / 10 .(130 = 13V)
    uint8_t		state;						// Charger state (1, 2, 3 or 4);
};
//const String CodeString = "evReg_Proto";          // String sent to TuningStudio
#define CodeString  "MSII s-Charger B-1  "       	// String sent to TuningStudio, use F() macro to store in progmem, not in sram.

#define CodeRev  "Smedby simple Charger Beta 1"		// Code Revition to send to Megatune. Title Bar in tunerStudio
#define RTPS  12                                    // Real time data page size
#define PG1S  81                                    // Eprom Page1 size
#define PG1Offset 0									// Offset in EEPROM where pg1 is stored



// MegaTune serial comunication class
class sMegaTune
{
public:
//	RPageVarsStruct RPage;
//	Page1DataStruct pg1;

private:
	int	sec;
	sSerial MySerial;
	sEEPROM MyEEPROM;
	char* pg1p;

//  int _BatteryVoltage;
//  String _serialCommand;

  public:
	sMegaTune(int);							// Setup serial port speed for MegaTune communication.
	void send_Sec(int);						// Send number of seconds passed
	void send_Rev(char*);					// Send code revision.
	void send_EpromVar();			// Send EEprom variables.
	void get_EpromVar(char* s);			// Get EEprom variables.
	void burn_EpromVars();					// Burn pg1 to EEPROM
	void load_EpromVars();					// Load pg1-vars from EEPROM, only done at startup.
	void send_RPage();						// Send realtime data
//  void send_RPage(uint8_t *rPage);              // Send realtime data

	void processSerial();
	bool dataRecived();

};



#endif /* SMEOS_MIDLEWARE_INCLUDE_SMEGATUNE_HPP_ */
