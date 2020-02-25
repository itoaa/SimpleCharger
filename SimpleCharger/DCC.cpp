/*
 * TunerStudio.cpp
 *
 *  Created on: 28 dec. 2018
 *      Author: Ola Andersson
 *
 *      Trying to gett communicatin ot tunerstudio from charger
 */

#include <stdlib.h>
#include <stdbool.h>
//#include <string.h>

#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <util/delay.h>

/* FreeRtos scheduler include files. */
#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* this is from avr/time.h, but it is not released into the main stream currently */
//#include "time.h"

// SmeOS include files

#include "sGPIO.hpp"
#include "sMegaTune.hpp"
#include "sADC.hpp"
#include "sSerial.hpp"
#include "sPWM.hpp"
#include "sVolt.hpp"
#include "sCurrent.hpp"

// Charger specific include files

#include "Hardware.hpp"


sGPIO green(GreenLedPport,GreenLedPin,1);
//sGPIO yelow(YellowLedPort, YellowLedPin,1);
//sGPIO red(RedLedPort,RedLedPin,1);

static void TaskComm(void *pvParameters);
static void TaskCharger(void *pvParameters); // Main charger task

RPageVarsStruct RPage;
Page1DataStruct pg1;

// sSerial MySerial(cBaudRate);

/* Main program loop */
//int main(void) __attribute__ ((OS_main));
int main(void)
{
//	puts("TunerStudio starting ");
	xTaskCreate(
		TaskCharger
		,  (const portCHAR *)"ChargeTask" // Main Arduino Mega 2560, Freetronics EtherMega (Red) LED Blink
		,  225				// Tested 64 free @ 228
		,  NULL
		,  3
		,  NULL ); //

    xTaskCreate(
		TaskComm
		,  (const portCHAR *)"CommunicationTask" // Task/function doing all serial communication
		,  208				// chack 9 free @ 208?
		,  NULL
		,  3
		,  NULL ); //

	vTaskStartScheduler();
	while (1)
	{
	}
}



static void TaskCharger(void *pvParameters) // Main charger task
{
	(void) pvParameters;
	// TickType_t xLastWakeTime;

    int IV,OV,IC,OC;
    int p = 3;
    Current outputCurrent(OutputCurrentPin,Current_Measure_type);
    outputCurrent.zeroAmpCallibrate();

    Current inputCurrent(7,1);
 //   inputCurrent.zeroAmpCallibrate();

    sPWM MyPWM(1,1);
//	Volt driverVolt(Pin12V,201);
	Volt outputVolt(OutputVoltPin,8000);
	Volt inputVolt(InputVoltPin,8000);
    for(;;)
    {

    	OV = outputVolt.readVolt();
    	IV = inputVolt.readVolt();
    	IC = inputCurrent.readCurrent();
    	OC = outputCurrent.readCurrent();

    	if (IC < 0) IC = 0 - IC;
    	if (OC < 0) OC = 0 - OC;

 //   	IP = (IV/10.0)*(IC/100.0);
 //   	OP = (OV/10.0)*(OC/100.0);

 //   	v = OP/IP*100.0;

    	vTaskSuspendAll();
    	RPage.InputVolt = IV;
    	RPage.InputAmp = IC;
    	RPage.OutputVolt = OV;
    	RPage.OutputAmp = OC;
    	RPage.pw1 = 254;
    	RPage.seconds= 10;
    	xTaskResumeAll();

    	if (OC < ((int)pg1.ChargeAmp*1000))
    	{
    		p++;
    	}
    	if (OC > ((int)pg1.ChargeAmp*1000))
    	{
    		p--;
    	}
    	if (p < 1) {p = 1;}
    	if (p > 1000) {p = 1000;}

    	MyPWM.setDuty(p);
    	OCR1A = p;

    	vTaskDelay( ( 50 / portTICK_PERIOD_MS ) );

   } // for loop ??
}
static void TaskComm(void *pvParameters)
{
    (void) pvParameters;

    TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	sMegaTune MT(1);

	for(;;)
    {
//		xSerialxPrintf_P(&xSerialPort, PSTR("Green: Current Timestamp: %lu xTaskGetTickCount(): %u\r\n"), time(NULL), xTaskGetTickCount());
//		xSerialxPrintf_P(&xSerialPort, PSTR("Green: Minimum Free Heap Size: %u\r\n"), xPortGetMinimumEverFreeHeapSize() ); // needs heap_4 for this function to succeed.
//    	xSerialxPrintf_P(&xSerialPort, PSTR("SerialSend: Stack High Water Mark, serial task: %u\r\n"), uxTaskGetStackHighWaterMark( NULL ) ); // Check stack Margin.
		if (MT.dataRecived()==1)
		{
			MT.processSerial();
		}

        vTaskDelayUntil( &xLastWakeTime, ( 11 / portTICK_PERIOD_MS ) );
	} // For loop
}






