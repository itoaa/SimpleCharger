/*
 * SoftSerialDebug.cpp
 *
 *  Created on: 1 dec. 2018
 *      Author: ola
 */
/*
 * main.cpp
 *
 *  Created on: 21 okt. 2018
 *      Author: ola
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* this is from avr/time.h, but it is not released into the main stream currently */
#include "time.h"

#include "sGPIO.hpp"
#include "sSerial.hpp"
#include "smcp_can.hpp"

extern "C"
{
	#include "dbg_putchar.h"
}

/*-----------------------------------------------------------*/
/* Create reference to the handle for the serial port. */

// define what pins the LEDs are connected to.
// in reality, PD6 is really just '6'
/*
#define GREEN_LED_PORT	1	// DDRB
#define GREEN_LED_PIN	2

#define YELOW_LED_PORT	2	// DDRC
#define YELOW_LED_PIN	4

#define RED_LED_PORT	3	//DDRD
#define RED_LED_PIN		7
*/
#define GREEN_LED_PORT	3	// DDRB
#define GREEN_LED_PIN	7

#define YELOW_LED_PORT	3	// DDRC
#define YELOW_LED_PIN	7

#define RED_LED_PORT	3	//DDRD
#define RED_LED_PIN		7

// static void TaskCharger(void *pvParameters);  // Main Charger task

// static void TaskComm(void *pvParameters);
/*-----------------------------------------------------------*/

// TickType_t xLastWakeTimei;

/*
sGPIO green(GREEN_LED_PORT,GREEN_LED_PIN,1);
sGPIO yelow(YELOW_LED_PORT,YELOW_LED_PIN,1);
sGPIO red(RED_LED_PORT,RED_LED_PIN,1);
*/

/* Main program loop */
//int main(void) __attribute__ ((OS_main));


int main(void)
{
	uint8_t c = 'A';

	dbg_tx_init();

	while(1) {
		dbg_putchar(c);
		if(++c > 'Z') c = 'A';
	}

	return 0;


	sSerial MySerial(19200);
    puts("Seting upp LED:s");
//	yelow.setLow();
//    red.setHigh();
//    green.setLow();
    puts("Seting upp CAN");
    MCP_CAN CAN0(PD2);
    puts("Starting CAN");
	if (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ)) { puts("Can Begin OK"); }
    puts("CAN Begin 500Kbps 16MHZ");
	CAN0.setMode(MCP_NORMAL);
	puts("CAN Mode set OK ...");
	cli();

	uint8_t data[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};

	while (1)
	{
		  // send data:  ID = 0x100, Standard CAN Frame, Data length = 8 bytes, 'data' = array of data bytes to send
		  uint8_t sndStat = CAN0.sendMsgBuf(0x100, 0, 8, data);
		  if(sndStat == CAN_OK){
		    puts("Message Sent Successfully!");
		  } else {
		    puts("Error Sending Message...");
		  }
		  _delay_ms(1000);   // send data per 100ms

	}

/*
	xTaskCreate(
		TaskCharger
		,  (const portCHAR *)"ChargeTask" // Main Arduino Mega 2560, Freetronics EtherMega (Red) LED Blink
		,  128				// Tested 9 free @ 208
		,  NULL
		,  3
		,  NULL ); //

    xTaskCreate(
		TaskComm
		,  (const portCHAR *)"CommunicationTask" // Task/function doing all serial communication
		,  128				// chack 9 free @ 208?
		,  NULL
		,  3
		,  NULL ); //

	vTaskStartScheduler();
*/
}


/*
static void TaskCharger(void *pvParameters) // Main charger task
{
    (void) pvParameters;
    TickType_t xLastWakeTime;
    for(;;)
    {
    	green.setLow();
    	puts("Green LED ON");
    	vTaskDelayUntil( &xLastWakeTime, ( 3000 / portTICK_PERIOD_MS ) );
    	green.setHigh();
    	puts("Green LED OFF");
    	vTaskDelayUntil( &xLastWakeTime, ( 1000 / portTICK_PERIOD_MS ) );

   } // for loop ??
}
static void TaskComm(void *pvParameters)
{
    (void) pvParameters;
    TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();


    for(;;)
    {
//		xSerialxPrintf_P(&xSerialPort, PSTR("Green: Current Timestamp: %lu xTaskGetTickCount(): %u\r\n"), time(NULL), xTaskGetTickCount());
//		xSerialxPrintf_P(&xSerialPort, PSTR("Green: Minimum Free Heap Size: %u\r\n"), xPortGetMinimumEverFreeHeapSize() ); // needs heap_4 for this function to succeed.
//    	xSerialxPrintf_P(&xSerialPort, PSTR("SerialSend: Stack High Water Mark, serial task: %u\r\n"), uxTaskGetStackHighWaterMark( NULL ) ); // Check stack Margin.

        red.setLow();
        vTaskDelayUntil( &xLastWakeTime, ( 100 / portTICK_PERIOD_MS ) );
        red.setHigh();
        yelow.switchState();
        puts("Switching Yelow led");
        vTaskDelayUntil( &xLastWakeTime, ( 340 / portTICK_PERIOD_MS ) );

	} // For loop
}


*/




