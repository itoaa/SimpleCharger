/*
 * main.cpp
 *
 *  Created on: 4 juli 2019
 *      Author: ola
 */

/* FreeRtos scheduler include files. */
#include <sMegaTune.hpp>
#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
// SmeOS include files

#include "sGPIO.hpp"
#include "sSerial.hpp"
#include "sPWM.hpp"
#include "sVolt.hpp"
#include "sCurrent.hpp"
#include "sOneWire.hpp"
#include "Hardware.hpp"
#include "eeprom.h"

#include <stdio.h>

sGPIO green(GreenLedPport,GreenLedPin,1);
sGPIO yelow(YellowLedPort, YellowLedPin,1);
sGPIO red(RedLedPort,RedLedPin,1);

sMegaTune MyMega(9600);

OneWire MyOneWire(TemperaturPort,TemperaturPin);

Current outputCurrent(OutputCurrentPin,Current_Measure_type);

sPWM MyPWM(pwmPort,pwmPin);

//	Volt driverVolt(Pin12V,201);				// May save memory to comment out

Volt outputVolt(OutputVoltPin,55);
Volt inputVolt(InputVoltPin,65);
Volt fetDriverVolt(Pin12V,202);



static void TaskCharger(void *pvParameters); // Main charger task
static void TaskCom(void *pvParameters); // Main communication task
static void TaskTemp(void *pvParameters); // Onewire temp task

int main()
{
		// Turn off LED:s
		red.setHigh();
		green.setHigh();
		yelow.setHigh();


		xTaskCreate(
			TaskCharger
			,  (const portCHAR *)"ChargeTask" // Main charger task
			,  290				//
			,  NULL
			,  3
			,  NULL ); //

		xTaskCreate(
			TaskCom
			,  (const portCHAR *)"ComTask" // Main charger task
			,  190				//
			,  NULL
			,  3
			,  NULL ); //

		xTaskCreate(
			TaskTemp
			,  (const portCHAR *)"tempTask" // OneWire task
			,  128				//
			,  NULL
			,  3
			,  NULL ); //

		vTaskStartScheduler();
		while (1)
		{
		}
}

static void TaskTemp(void *pvParameters) // Main charger task
{
	(void) pvParameters;
	uint8_t type_s;
	uint8_t data[12];
	uint8_t addr[8];
	uint8_t i;
	float celsius;

	if ( !MyOneWire.search(addr))
	{
		// No more addresses
		MyOneWire.reset_search();
		vTaskDelay( ( 250 / portTICK_PERIOD_MS ) );
		return;
	}
	// the first ROM byte indicates which chip
	switch (addr[0])
	{
		case 0x10:
	      // DS18S20 or old DS1820
	      type_s = 1;
	      break;
	    case 0x28:
	      // DS18B20
	      type_s = 0;
	      break;
	    case 0x22:
	      // DS1822
	      type_s = 0;
	      break;
	    default:
	      return;
	}

    for(;;)
    {
    	  MyOneWire.reset();
    	  MyOneWire.select(addr);
    	  MyOneWire.write(0x44, 1);        // start conversion, with parasite power on at the end

    	  vTaskDelay( ( 1000 / portTICK_PERIOD_MS ) );     // maybe 750ms is enough, maybe not

    	  MyOneWire.reset();
    	  MyOneWire.select(addr);
    	  MyOneWire.write(0xBE);         // Read Scratchpad

    	  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    	     data[i] = MyOneWire.read();
    	  }
    	  int16_t raw = (data[1] << 8) | data[0];
    	  if (type_s) {
    	    raw = raw << 3; // 9 bit resolution default
    	    if (data[7] == 0x10) {
    	      // "count remain" gives full 12 bit resolution
    	      raw = (raw & 0xFFF0) + 12 - data[6];
    	    }
    	  } else {
    	    uint8_t cfg = (data[4] & 0x60);
    	    // at lower res, the low bits are undefined, so let's zero them
    	    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    	    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    	    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    	    //// default is 12 bit resolution, 750 ms conversion time
    	  }
    	  celsius = (float)raw / 16.0;
//    	  fahrenheit = celsius * 1.8 + 32.0;
    	  MyMega.RPage.mosfetTemp = (int8_t)celsius;

    	vTaskDelay( ( 10 / portTICK_PERIOD_MS ) );
    }
}

static void TaskCom(void *pvParameters) // Main charger task
{
	(void) pvParameters;
    for(;;)
    {
		MyMega.processSerial();
    	vTaskDelay( ( 10 / portTICK_PERIOD_MS ) );
    }
}

static void TaskCharger(void *pvParameters) // Main charger task
{
	(void) pvParameters;

	vTaskDelay( ( 200 / portTICK_PERIOD_MS ) );		// Wait 200mS before zeroing the current
	outputCurrent.zeroAmpCallibrate();
	vTaskDelay( ( 10 / portTICK_PERIOD_MS ) );

	int8_t AmpOut;
	uint8_t pwm = 0;
	MyMega.pg1.MaxPWM = 0;
    for(;;)
    {
    	if (MyMega.RPage.state == 0)
    	{
        	green.setLow();
        	MyMega.RPage.mosfetDriverVolt = fetDriverVolt.readVolt();
        	MyMega.RPage.OutputVolt = outputVolt.readVolt() ;
        	AmpOut = (int8_t)(outputCurrent.readCurrent()/ 100);
        	if (AmpOut < 0) AmpOut = 0 - AmpOut;		// Only positive Amp values on this charger.
        	MyMega.RPage.OutputAmp = AmpOut;
        	MyMega.RPage.InputVolt = inputVolt.readVolt() ;
        	MyMega.RPage.InputAmp = (int8_t)(((MyMega.RPage.OutputVolt * MyMega.RPage.OutputAmp) / MyMega.RPage.InputVolt)*1.2);	// estimate input current.

        	// Check for errors (Later)
        	// Set charge current (with "soft start")
        	do
        	{
        		AmpOut = (int8_t)(outputCurrent.readCurrent()/ 100);
            	if (AmpOut < 0) AmpOut = 0 - AmpOut;		// Only positive Amp values on this charger.
            	MyMega.RPage.OutputAmp = AmpOut;
            	red.setLow();
            	if ( (AmpOut < MyMega.pg1.ChargeAmp) && (pwm < 254) )
            	{
        			pwm++;
                	vTaskDelay( ( 10 / portTICK_PERIOD_MS ) );
            	}
        		if ( (AmpOut > MyMega.pg1.ChargeAmp) && (pwm > 0x00) )
        			pwm--;
        		MyMega.RPage.pw1 = pwm;
        		MyPWM.setDuty(pwm);
            	vTaskDelay( ( 5 / portTICK_PERIOD_MS ) );

//        	}while (AmpOut != MyMega.pg1.ChargeAmp);        // I_FAST is set now
        	}while ( (AmpOut <= MyMega.pg1.ChargeAmp-2) or (AmpOut >= MyMega.pg1.ChargeAmp +2) );        // I_FAST is set now

        		red.setHigh();


        	//   		MyPWM.setDuty(MyMega.pg1.MaxPWM);

        	green.setHigh();

        	vTaskDelay( ( 50 / portTICK_PERIOD_MS ) );

    	}
    	if (MyMega.RPage.state == 2)
    	{

    	}


    } // for loop ??


}
