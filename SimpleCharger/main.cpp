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



static void TaskCharger(void *pvParameters); // Main charger task




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
			,  296				//
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
//    uint16_t IV,OV;
 //   uint16_t pot;
//    int16_t IC,OC;
//    uint16_t p = 100;
    //    float IP,OP,v;

    Current outputCurrent(OutputCurrentPin,Current_Measure_type);
    outputCurrent.zeroAmpCallibrate();

    Current inputCurrent(7,1);
   	vTaskDelay( ( 200 / portTICK_PERIOD_MS ) );			// Wait until everything has started up, and then calibrate for 0 amp
   	inputCurrent.zeroAmpCallibrate();					// Calibrate as 0 amp. Charger will still draining some current
   													 	// but to get efficiency value more realistic, we ignore static drain.

    sPWM MyPWM(pwmPort,pwmPin);

    //	Volt driverVolt(Pin12V,201);

    Volt outputVolt(OutputVoltPin,55);
	Volt inputVolt(InputVoltPin,65);

	Volt potVolt(PC6,3000);
	sMegaTune MyMega(9600);

	MyMega.pg1.MaxPWM = 0;
    for(;;)
    {
    	green.setLow();
//    	pot = potVolt.readVolt();

    	MyMega.RPage.OutputVolt = outputVolt.readVolt() ;
    	MyMega.RPage.InputVolt = inputVolt.readVolt() ;

    	MyMega.RPage.InputAmp = inputCurrent.readCurrent() / 100;
    	MyMega.RPage.OutputAmp = outputCurrent.readCurrent() / 100;


    	if (MyMega.RPage.InputAmp < 0) MyMega.RPage.InputAmp = 0 - MyMega.RPage.InputAmp;
    	if (MyMega.RPage.OutputAmp < 0) MyMega.RPage.OutputAmp = 0 - MyMega.RPage.OutputAmp;


   		MyPWM.setDuty(MyMega.pg1.MaxPWM);
 //   	MyPWM.setDuty(p);
    	OCR1A = MyMega.pg1.MaxPWM * 10;

    	MyMega.processSerial();
    	green.setHigh();

    	vTaskDelay( ( 100 / portTICK_PERIOD_MS ) );


   } // for loop ??


}
