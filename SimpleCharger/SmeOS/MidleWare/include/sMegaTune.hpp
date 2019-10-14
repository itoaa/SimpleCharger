/*
 * sMegaTuneSimp10.hpp
 *
 *  Created on: 28 dec. 2018
 *      Author: ola
 */

#ifndef SMEOS_MIDLEWARE_INCLUDE_SMEGATUNE_HPP_
#define SMEOS_MIDLEWARE_INCLUDE_SMEGATUNE_HPP_

#include <stdint.h>
#include "sSerial.hpp"
#include <FreeRTOS.h>
#include <avr/io.h>
#include <task.h>


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

};
// The structure of the realtime data sent on the Serial port to megatune
struct __attribute__((packed,aligned(1))) RPageVarsStruct
{
    uint8_t		seconds;					// Seconds
    uint8_t		pw1;						// Pulse with (PWM)
    uint8_t		rpm;						// RPM on motor
    uint8_t		TPSPos;						// Throttle Position (0-100)
    int8_t		InputAmp;					// input Current
    int8_t		InputVolt;					// Input Volt
    int8_t		OutputAmp;					// Output Current
    int8_t    OutputVolt;					// Output Volt
    int8_t    MosfetTempCor;                // MosFet Teperatur.
    int8_t    MosfetTempADC;                // MosFet Teperatur.
    int8_t    MosfetDriverVolt;				// Mosfet driver volt / 10 .(130 = 13V)
};
//const String CodeString = "evReg_Proto";          // String sent to TuningStudio
#define CodeString  "MSII s-Charger B-1  "       	// String sent to TuningStudio, use F() macro to store in progmem, not in sram.

#define CodeRev  "Smedby simple Charger Beta 1"		// Code Revition to send to Megatune. Title Bar in tunerStudio
#define RTPS  11                                    // Real time data page size
#define PG1S  33                                    // Eprom Page1 size




// MegaTune serial comunication class
class sMegaTune
{
public:
	RPageVarsStruct RPage;
	Page1DataStruct pg1;

private:
	int	sec;
	sSerial MySerial;


//  int _BatteryVoltage;
//  String _serialCommand;

  public:
	sMegaTune(int);							// Setup serial port speed for MegaTune communication.
	void send_Sec(int);                        // Send number of seconds passed
  void send_Rev(char*);                      // Send code revision.
  void send_EpromVar(char *pg1);             // Send EEprom variables.
  void get_EpromVar(char *pg1);              // Get EEprom variables.
  void send_RPage();              // Send realtime data
//  void send_RPage(uint8_t *rPage);              // Send realtime data

  void processSerial();
  bool dataRecived();

};



#endif /* SMEOS_MIDLEWARE_INCLUDE_SMEGATUNE_HPP_ */
