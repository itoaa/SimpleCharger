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
#include "Hardware.hpp"

#include <stdio.h>

sGPIO green(GreenLedPport,GreenLedPin,1);
sGPIO yelow(YellowLedPort, YellowLedPin,1);
sGPIO red(RedLedPort,RedLedPin,1);

sMegaTune MyMega(9600);


static void TaskCharger(void *pvParameters); // Main charger task
static void TaskCom(void *pvParameters); // Main communication task




int main()
{
//		sSerial MySerial(9600);
//	    puts("Starting Serial...");
//	    puts("Speed is 9600...");
	   // Setup LED:s
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


		vTaskStartScheduler();
		while (1)
		{
		}
}

static void TaskCom(void *pvParameters) // Main charger task
{
	(void) pvParameters;
//	MyMega.processSerial();
	for(;;)
	    {
	vTaskDelay( ( 200 / portTICK_PERIOD_MS ) );
	    }
}

static void TaskCharger(void *pvParameters) // Main charger task
{
	(void) pvParameters;

	red.setLow();

   	vTaskDelay( ( 1000 / portTICK_PERIOD_MS ) );

    red.setHigh();

    for(;;)
    {
    	green.setLow();
       	vTaskDelay( ( 10 / portTICK_PERIOD_MS ) );

    	green.setHigh();

    	vTaskDelay( ( 900 / portTICK_PERIOD_MS ) );


    } // for loop ??


}
