/*
 * sDB.h
 *
 *  Created on: 29 Feb 2020
 *      Author: olaandersson
 */

#ifndef SIMPLECHARGER_SMEOS_MIDLEWARE_INCLUDE_SDB_HPP_
#define SIMPLECHARGER_SMEOS_MIDLEWARE_INCLUDE_SDB_HPP_

#include <stdint.h>
#include <sEEPROM.hpp>

#define RTPS  16                                    // Real time data page size
#define PG1S  81                                    // Eprom Page1 size
#define PG1Offset 0									// Offset in EEPROM where pg1 is stored

class sDB {
public:
	sDB();

	uint8_t getpg1Data(uint8_t);
	void setpg1Data(uint8_t offset, uint8_t data);

	uint8_t getrtpData(uint8_t);
	void setrtpData(uint8_t offset, uint8_t data);

	void savePG1();
	void loadPG1();
private:
	sEEPROM MyEEPROM;

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
	    uint8_t		process1Stack;				// Free stack for process 1
	    uint8_t		process2Stack;				// Free stack for process 2
	    uint8_t		process3Stack;				// Free stack for process 3
	    uint8_t		process4Stack;				// Free stack for process 4

	};

	struct __attribute__((packed,aligned(1))) Page1Datastruct
	{
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
public:
	Page1Datastruct pg1;
	RPageVarsStruct rtPage;
};

extern sDB GlobalDB;

#endif /* SIMPLECHARGER_SMEOS_MIDLEWARE_INCLUDE_SDB_HPP_ */
