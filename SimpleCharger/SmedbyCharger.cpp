/*
 * main.cpp
 *
 *  Created on: 4 juli 2019
 *      Author: ola
 */

#define ChargeTask
#define ComTask
#define TempTask

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
sGPIO red(RedLedPort,RedLedPin,1);				// Charger red led
sGPIO debug1(1,PB5,1);				// Arduino UNO led

sMegaTune MyMega(9600);

OneWire MyOneWire(TemperaturPort,TemperaturPin);

Current outputCurrent(OutputCurrentPin,Current_Measure_type);

sPWM MyPWM(pwmPort,pwmPin);

//	Volt driverVolt(Pin12V,201);				// May save memory to comment out

Volt outputVolt(OutputVoltPin,55);
Volt inputVolt(InputVoltPin,65);
Volt fetDriverVolt(Pin12V,202);

#ifdef ChargeTask
static void TaskCharger(void *pvParameters); // Main charger task
# endif
#ifdef ComTask
static void TaskCom(void *pvParameters); // Main communication task
# endif
# ifdef TempTask
static void TaskTemp(void *pvParameters); // Onewire temp task
# endif

int main()
{
		// Turn off LED:s
		red.setHigh();
		green.setHigh();
		yelow.setHigh();
		debug1.setHigh();

# ifdef ChargeTask
		xTaskCreate(
			TaskCharger
			,  (const portCHAR *)"ChargeTask" // Main charger task
			,  160				//
			,  NULL
			,  3
			,  NULL ); //
# endif
# ifdef ComTask
		xTaskCreate(
			TaskCom
			,  (const portCHAR *)"ComTask" // Main charger task
			,  100				//
			,  NULL
			,  3
			,  NULL ); //
# endif
# ifdef TempTask
		xTaskCreate(
			TaskTemp
			,  (const portCHAR *)"tempTask" // OneWire task
			,  128				//
			,  NULL
			,  3
			,  NULL ); //
# endif
		vTaskStartScheduler();
		while (1)
		{
		}
}
# ifdef TempTask
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
    	  GlobalDB.rtPage.mosfetTemp = (int8_t)celsius;

    	vTaskDelay( ( 10 / portTICK_PERIOD_MS ) );
    }
}
# endif
static void TaskCom(void *pvParameters) // Main charger task
{
	(void) pvParameters;
    for(;;)
    {
		debug1.setLow();

    	GlobalDB.rtPage.process1Stack = uxTaskGetStackHighWaterMark( NULL );
    	MyMega.processSerial();
    	vTaskDelay( ( 10 / portTICK_PERIOD_MS ) );

    	debug1.setHigh();

    	vTaskDelay( ( 10 / portTICK_PERIOD_MS ) );
    }

}

static void TaskCharger(void *pvParameters) // Main charger task
{
	(void) pvParameters;

	vTaskDelay( ( 200 / portTICK_PERIOD_MS ) );		// Wait 200mS before zeroing the current
//	outputCurrent.zeroAmpCallibrate();
	vTaskDelay( ( 10 / portTICK_PERIOD_MS ) );

	int8_t AmpOut;

    for(;;)
    {
    	if (GlobalDB.rtPage.state == 0)
    	{

    		green.setLow();


        	GlobalDB.rtPage.mosfetDriverVolt = fetDriverVolt.readVolt();
        	GlobalDB.rtPage.OutputVolt = outputVolt.readVolt() ;
//        	AmpOut = (int8_t)(outputCurrent.readCurrent()/ 100);
//        	if (AmpOut < 0) AmpOut = 0 - AmpOut;		// Only positive Amp values on this charger.
        	GlobalDB.rtPage.OutputAmp = AmpOut;
        	GlobalDB.rtPage.InputVolt = inputVolt.readVolt() ;
        	GlobalDB.rtPage.InputAmp = (int8_t)(((GlobalDB.rtPage.OutputVolt * GlobalDB.rtPage.OutputAmp) / GlobalDB.rtPage.InputVolt)*1.2);	// estimate input current.
red.setLow();

//vTaskDelay( ( 50 / portTICK_PERIOD_MS ) );


        	// Check for errors (Later)
        	// Set charge current (with "soft start")
/*
        	do
        	{
//        		AmpOut = (int8_t)(outputCurrent.readCurrent()/ 100);
            	if (AmpOut < 0) AmpOut = 0 - AmpOut;		// Only positive Amp values on this charger.
            	GlobalDB.rtPage.OutputAmp = AmpOut;
            	red.setLow();
            	if ( (AmpOut < GlobalDB.pg1.ChargeAmp) && (pwm < 254) )
            	{
        			pwm++;
                	vTaskDelay( ( 10 / portTICK_PERIOD_MS ) );
            	}
        		if ( (AmpOut > GlobalDB.pg1.ChargeAmp) && (pwm > 0x00) )
        			pwm--;
        		GlobalDB.rtPage.pw1 = pwm;
        		MyPWM.setDuty(pwm);
            	vTaskDelay( ( 5 / portTICK_PERIOD_MS ) );

//        	}while (AmpOut != MyMega.pg1.ChargeAmp);        // I_FAST is set now
        	}while ( (AmpOut <= GlobalDB.pg1.ChargeAmp-2) or (AmpOut >= GlobalDB.pg1.ChargeAmp +2) );        // I_FAST is set now
*/
       		red.setHigh();
        	//   		MyPWM.setDuty(MyMega.pg1.MaxPWM);

       		green.setHigh();
       		GlobalDB.rtPage.process2Stack = uxTaskGetStackHighWaterMark( NULL );

       		vTaskDelay( ( 50 / portTICK_PERIOD_MS ) );
    	}
    	if (GlobalDB.rtPage.state == 2)
    	{

    	}

    	vTaskDelay( ( 50 / portTICK_PERIOD_MS ) );
    } // for loop ??


}


