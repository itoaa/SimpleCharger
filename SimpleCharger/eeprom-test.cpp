/*
 * eeprom-test.cpp
 *
 *  Created on: 22 feb. 2020
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
#include "sEEPROM.hpp"

// #include "avr/eeprom.h"

#include <stdio.h>


static void TaskCharger(void *pvParameters); // Main charger task
static void TaskCom(void *pvParameters); // Main communication task
static void TaskTemp(void *pvParameters); // Onewire temp task

sSerial MySerial;
sEEPROM MyEEPROM;

int main()
{
		// Turn off LED:s


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
	MySerial.init(1);
	printf("%s","starting... \n\r");

	uint8_t st = 0;
	for(uint16_t i=0; i<10; i++)
	{
		printf("%s","Värde " );
		printf("%i",i);
		st = MyEEPROM.readByte(i);
		printf("%s"," = " );
		printf("%i",st);
		printf("\r\n" );

	}
	for(uint16_t i=0; i<10; i++)
	{
		printf("%s","Skriver värde " );
		printf("%i",i);
		MyEEPROM.writeByte(i, i+55);
		printf("%s"," till " );
		printf("%i",i);
		printf("\r\n" );

	}

	for(;;)
    {
		MySerial.putChar('B');

		vTaskDelay( ( 500 / portTICK_PERIOD_MS ) );
    }
}

static void TaskCom(void *pvParameters) // Main charger task
{
	(void) pvParameters;
    for(;;)
    {
    	vTaskDelay( ( 10 / portTICK_PERIOD_MS ) );
    }
}

static void TaskCharger(void *pvParameters) // Main charger task
{
	(void) pvParameters;

	vTaskDelay( ( 200 / portTICK_PERIOD_MS ) );		// Wait 200mS before zeroing the current

    for(;;)
    {

    } // for loop ??


}

