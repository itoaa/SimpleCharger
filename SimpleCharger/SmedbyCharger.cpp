/*
 * main.cpp
 *
 *  Created on: 4 juli 2019
 *      Author: ola
 */

#define ChargeT					// Enable ChargerTask. The task that makes the charger do what parameters say, and not burn up in the process
#define ComT					// Enable ComTask. The task that communicate with the out side world, without interfering.
#define TempT					// Enable TemTask. The task that only measure temp and update global db :-)
#define	AutoChargerT			// Enable AutoChargerTask. The task that make the charger work without external control.

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
#include <stdbool.h>

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

#ifdef ChargeT
static void ChargerTask(void *pvParameters); // Main charger task
# endif
#ifdef ComT
static void ComTask(void *pvParameters); // Main communication task
# endif
# ifdef TempT
static void TempTask(void *pvParameters); // Onewire temp task
# endif
# ifdef AutoChargerT
static void AutoChargerTask(void *pvParameters); // Onewire temp task
# endif

static void readAllValues();
static void setPWM();
static int Errors();
static bool OutputAmpInRange();
static bool noTomeOut(int milliSec);


int main()
{
		// Turn off LED:s
		red.setHigh();
		green.setHigh();
		yelow.setHigh();
		debug1.setHigh();

# ifdef ChargeT
		xTaskCreate(
			ChargerTask
			,  (const portCHAR *)"ChargerTask" // Main charger task
			,  145				//
			,  NULL
			,  3
			,  NULL ); //
# endif
# ifdef ComT
		xTaskCreate(
			ComTask
			,  (const portCHAR *)"ComTask" // Main charger task
			,  118				//
			,  NULL
			,  3
			,  NULL ); //
# endif
# ifdef TempT
		xTaskCreate(
			TempTask
			,  (const portCHAR *)"TempTask" // OneWire task
			,  120				//
			,  NULL
			,  3
			,  NULL ); //
# endif
# ifdef AutoChargerT
		xTaskCreate(
			AutoChargerTask
			,  (const portCHAR *)"AutoChargerTask" // Make the charger autonomous
			,  40				//
			,  NULL
			,  3
			,  NULL ); //
# endif




		vTaskStartScheduler();
		while (1)
		{
		}
}

# ifdef TempT
static void TempTask(void *pvParameters) // Main charger task
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

    	  GlobalDB.rtPage.process3Stack = uxTaskGetStackHighWaterMark( NULL );

    	  debug1.switchState();
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

    	  GlobalDB.rtPage.process3Stack = uxTaskGetStackHighWaterMark( NULL );

    	vTaskDelay( ( 10 / portTICK_PERIOD_MS ) );
    }
}
# endif

static void ComTask(void *pvParameters) // Main charger task
{
	(void) pvParameters;
    for(;;)
    {

    	MyMega.processSerial();
    	GlobalDB.rtPage.process1Stack = uxTaskGetStackHighWaterMark( NULL );

    	vTaskDelay( ( 10 / portTICK_PERIOD_MS ) );


    	vTaskDelay( ( 10 / portTICK_PERIOD_MS ) );
    }

}

static void ChargerTask(void *pvParameters) // Main charger task
{
	// This task is always running and is controlling that the charger is doing what the settings is suggesting.
	// Also controlling that the values is in range (not exceeds the limits of supply, batters or charger).
	(void) pvParameters;

	// Maybe check and turn of PWM hear
	vTaskDelay( ( 200 / portTICK_PERIOD_MS ) );				// Wait 200mS before zeroing the current (need to move to setup)
	outputCurrent.zeroAmpCallibrate();
	vTaskDelay( ( 100 / portTICK_PERIOD_MS ) );

	// Reset the charger

    for(;;)													// Ring charger task infinite
    {
//    	if (GlobalDB.rtPage.state == ChargerStateMonitor)	// If charger is in state Monitor
    	if (Errors() == 0 )										// Check if charger is suppose to run, = no errors
    	{
    		green.setLow();									// Charger is actively doing something, = green LED on
    		readAllValues();								// Read all Values, in/out volt, mosfet volt, amp ...

    		setPWM();										// Find right PWM value, for correct current or volt level

    		//vTaskDelay( ( 50 / portTICK_PERIOD_MS ) );
        	do
        	{
            	vTaskDelay( ( 5 / portTICK_PERIOD_MS ) );

//        	}while (AmpOut != MyMega.pg1.ChargeAmp);        // I_FAST is set now
//        	}while ( (AmpOut <= GlobalDB.pg1.ChargeAmp-2) or (AmpOut >= GlobalDB.pg1.ChargeAmp +2) );        // I_FAST is set now
    	}while ( 1 == 2 );

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

static void AutoChargerTask(void *pvParameters) // The task that make the charger autonomous
{
	// This task makes the charger work on its own, without outside control. Good for simple battery chargers and when communication with head unit is lost.
	// Initially used when TunerStudio is used to set charge cycles.
	(void) pvParameters;

    for(;;)
    {
    	// Make the charger work without external input
    }
}


static void readAllValues()
{
	int8_t AmpOut = 0;

	GlobalDB.rtPage.mosfetDriverVolt = fetDriverVolt.readVolt();
	GlobalDB.rtPage.OutputVolt = outputVolt.readVolt() ;
	AmpOut = (int8_t)(outputCurrent.readCurrent()/ 100);
	if (AmpOut < 0) AmpOut = 0 - AmpOut;		// Only positive Amp values on this charger.
	GlobalDB.rtPage.OutputAmp = AmpOut;
	GlobalDB.rtPage.InputVolt = inputVolt.readVolt() ;
	GlobalDB.rtPage.InputAmp = (int8_t)(((GlobalDB.rtPage.OutputVolt * GlobalDB.rtPage.OutputAmp) / GlobalDB.rtPage.InputVolt)*1.2);	// estimate input current.

}

static void setPWM()
{
	int mAmpOut = 0;
	int pwm = 0;
	red.setLow();											// Charger is finding right PWM value, = red LED on.

	if (GlobalDB.rtPage.state == ChargerStateConstantAmp)	// If charger is in constant current mode
	{
		mAmpOut = (outputCurrent.readCurrent());				// AmpOut in A * 10, 1A = 10, one decimal
		if (mAmpOut < 0) mAmpOut = 0 - mAmpOut;				// Only positive Amp values on this charger.
		GlobalDB.rtPage.OutputAmp = mAmpOut / 100;			// devide by 100 here to get mA to A / 10 = we only whant Amp with one decimal.
		red.setLow();										// Trying to find charge current
		while ( !OutputAmpInRange() and noTomeOut(10))
		{
			if ( (mAmpOut < GlobalDB.pg1.ChargeAmp) && (pwm < 254) )
			{
				pwm++;
			}
			if ( (mAmpOut > GlobalDB.pg1.ChargeAmp) && (pwm > 0x00) )
			{
				pwm--;
			}
			GlobalDB.rtPage.pw1 = pwm;
			MyPWM.setDuty(pwm);

			}
	}
}

static int Errors()
{
	return (0);
}

static bool OutputAmpInRange()
{
	return (0);
}

static bool noTomeOut(int timeoutms)
{
	//if curent ms > startms + timeoutms ....
	return (0);
}

