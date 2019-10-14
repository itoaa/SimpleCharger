/*
 * sMegaTune.hpp
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
    uint16_t TimeAmpTabel[8];
    uint16_t ChargeAmp;
    uint16_t MaxChargeAmp;
    uint16_t MaxChargeVolt;
    uint16_t MaxInputAmp;
    uint16_t MaxInputVolt;
    uint16_t MaxPWM;
    uint16_t MinPWM;
    uint16_t MaxTemp;
    uint16_t mosfetTempADCTable[12];
    int16_t  mosfetTempTable[12];
    uint16_t accDelay;
};
// The structure of the realtime data sent on the Serial port to megatune
struct __attribute__((packed,aligned(1))) RPageVarsStruct
{
    uint16_t    seconds,pw1,rpm;	            // pw in usec
    uint16_t    TPSPos;                         // TPS Position (0-100)
    int16_t    InputAmp;	                  	// input Current
    int16_t    InputVolt;						// Input Volt
    int16_t		OutputAmp;	                   // Output Current
    int16_t    OutputVolt;			            // Output Volt
    int16_t    MosfetTempCor;                // MosFet Teperatur.
    int16_t    MosfetDriverVolt;               // Mosfet driver volt. (9 - 15V? ...)
//    uint16_t    PWM;                            // PWM value
};
//const String CodeString = "evReg_Proto";          // String sent to TuningStudio
#define CodeString  "Simple charger 1.0     "       // String sent to TuningStudio, use F() macro to store in progmem, not in sram.

#define CodeRev  "MSII s-Charger 1.0  "             // Code Revition to send to Megatune.
#define RTPS  20                                    // Real time data page size
#define PG1S  82                                    // Eprom Page1 size

// MegaTune serial comunication class
class sMegaTune
{
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
  void send_RPage(uint8_t *rPage);              // Send realtime data
  void processSerial();
  bool dataRecived();

};



#endif /* SMEOS_MIDLEWARE_INCLUDE_SMEGATUNE_HPP_ */
