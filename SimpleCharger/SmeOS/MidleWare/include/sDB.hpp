/*
 * sDB.h
 *
 *  Created on: 29 Feb 2020
 *      Author: olaandersson
 */

#ifndef SIMPLECHARGER_SMEOS_MIDLEWARE_INCLUDE_SDB_HPP_
#define SIMPLECHARGER_SMEOS_MIDLEWARE_INCLUDE_SDB_HPP_

#include <stdint.h>

class sDB {
public:
	sDB();

	void get123();
	int variab;
private:
	 struct __attribute__((packed,aligned(1))) RPageVarsStruct
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

	 uint8_t getpg1Data(uint8_t);
	 void setpg1Data(uint8_t offset, uint8_t data);
	 RPageVarsStruct pg1;

};

extern sDB GlobalDB;

#endif /* SIMPLECHARGER_SMEOS_MIDLEWARE_INCLUDE_SDB_HPP_ */
