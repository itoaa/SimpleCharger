/*
 * HardwareSerial.cpp
 *
 *  Created on: 1 dec. 2018
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
#include "sADC.hpp"
#include "sSerial.hpp"
#include "smcp_can.hpp"
#include "sOneWire.hpp"

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

OneWire ds(1,2);


int main(void)
{
	sSerial MySerial(19200);
    puts("Starting Serial...");
    puts("Speed is 19200...");

    while (1)
	{
    	uint8_t i;
    	uint8_t present = 0;
    	uint8_t type_s;
    	uint8_t data[12];
    	uint8_t addr[8];
    	float celsius, fahrenheit;
    	if ( !ds.search(addr)) {
    	    puts("No more addresses.");
    	    printf("\n\r");
    	    ds.reset_search();
    	    _delay_ms(250);
    	    return(0);
    	  }

    	  printf("ROM =");
    	  for( i = 0; i < 8; i++) {
    	    printf(" ");
    	    printf("%02x",addr[i]);		// ,HEX
    	  }

    	  if (OneWire::crc8(addr, 7) != addr[7]) {
    	      printf("CRC is not valid!");
    	      return(0);
    	  }
    	  printf("\n\r");

    	  // the first ROM byte indicates which chip
    	  switch (addr[0]) {
    	    case 0x10:
    	      puts("  Chip = DS18S20");  // or old DS1820
    	      type_s = 1;
    	      break;
    	    case 0x28:
    	      puts("  Chip = DS18B20");
    	      type_s = 0;
    	      break;
    	    case 0x22:
    	      puts("  Chip = DS1822");
    	      type_s = 0;
    	      break;
    	    default:
    	      puts("Device is not a DS18x20 family device.");
    	      return(0);
    	  }

    	  ds.reset();
    	  ds.select(addr);
    	  ds.write(0x44, 1);        // start conversion, with parasite power on at the end

    	  _delay_ms(1000);     // maybe 750ms is enough, maybe not
    	  // we might do a ds.depower() here, but the reset will take care of it.

    	  present = ds.reset();
    	  ds.select(addr);
    	  ds.write(0xBE);         // Read Scratchpad

    	  printf("  Data = ");
    	  printf("%02x",present);		// , HEX
    	  printf(" ");
    	  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    	    data[i] = ds.read();
    	    printf("%02x",data[i]);		// , HEX
    	    printf(" ");
    	  }
    	  printf(" CRC=");
    	  printf("%02x", OneWire::crc8(data, 8));
    	  printf("\n\r");

    	  // Convert the data to actual temperature
    	  // because the result is a 16 bit signed integer, it should
    	  // be stored to an "int16_t" type, which is always 16 bits
    	  // even when compiled on a 32 bit processor.
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
    	  fahrenheit = celsius * 1.8 + 32.0;
    	  printf("  Temperature = ");
    	  printf("%d",(int)celsius);
    	  printf(" Celsius, ");
    	  printf("%d",(int)fahrenheit);
    	  printf(" Fahrenheit\n\r");
	}

    while (1)
    {

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


