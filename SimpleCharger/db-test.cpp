/*
 * main.cpp
 *
 *  Created on: 29 feb 2020
 *      Author: ola
 */

/* FreeRtos scheduler include files. */
#include <sDB.hpp>
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
#include "sDB.hpp"

sGPIO green(GreenLedPport,GreenLedPin,1);
sGPIO yelow(YellowLedPort, YellowLedPin,1);
sGPIO red(RedLedPort,RedLedPin,1);

// sMegaTune MyMega(9600);

OneWire MyOneWire(TemperaturPort,TemperaturPin);

Current outputCurrent(OutputCurrentPin,Current_Measure_type);

//sPWM MyPWM(pwmPort,pwmPin);

//	Volt driverVolt(Pin12V,201);				// May save memory to comment out

//Volt outputVolt(OutputVoltPin,55);
//Volt inputVolt(InputVoltPin,65);
//Volt fetDriverVolt(Pin12V,202);

RPageVarsStruct RPage;
//Page1DataStruct pg1;

sSerial MySerial(9600);


static void taskA(void *pvParameters); // Main charger task
static void taskB(void *pvParameters); // Main communication task
static void taskC(void *pvParameters); // Onewire temp task

int main()
{
	sSerial MySerial(9600);
		// Turn off LED:s
	MySerial.init(9600);
		red.setHigh();
		green.setHigh();
		yelow.setHigh();
		GlobalDB.pg2.ChargeAmp = 2;
		printf("Starting ...");

		xTaskCreate(
			taskA
			,  (const portCHAR *)"tastA" // Main charger task
			,  170		//
			,  NULL
			,  3
			,  NULL ); //

		xTaskCreate(
			taskB
			,  (const portCHAR *)"taskB" // Main charger task
			,  100					//
			,  NULL
			,  3
			,  NULL ); //

		xTaskCreate(
			taskC
			,  (const portCHAR *)"taskC" // OneWire task
			,  100		//
			,  NULL
			,  3
			,  NULL ); //

		vTaskStartScheduler();
		while (1)
		{
		}
}

static void taskA(void *pvParameters) // Main charger task
{
	(void) pvParameters;
	uint8_t i;
    for(;;)
    {
    	vTaskSuspendAll();
    	i = GlobalDB.pg2.ChargeAmp;
    	printf("Laddström = %d ",i);
 //   	vTaskDelay( ( 10 / portTICK_PERIOD_MS ) );
    	printf("index 10 = %d ",GlobalDB.getpg1Data(10));
    	xTaskResumeAll();
        	vTaskDelay( ( 1000 / portTICK_PERIOD_MS ) );
    }
}

static void taskB(void *pvParameters) // Main charger task
{
	(void) pvParameters;

    for(;;)
    {
    	vTaskDelay( ( 10 / portTICK_PERIOD_MS ) );
    }
}

static void taskC(void *pvParameters) // Main charger task
{
	(void) pvParameters;
	int dir = 1;
    for(;;)
    {
    	if (GlobalDB.pg2.ChargeAmp >= 50) dir = 2;
    	if (GlobalDB.pg2.ChargeAmp <= 5) dir = 1;


    	if (dir == 2)
    		{
    		GlobalDB.pg2.ChargeAmp --;
    		GlobalDB.setpg1Data(10,10);
    		}
    	if (dir == 1)
    		{
    		GlobalDB.pg2.ChargeAmp ++;
    		GlobalDB.setpg1Data(10,20);

    		}

    	vTaskDelay( ( 100 / portTICK_PERIOD_MS ) );
    }
}
