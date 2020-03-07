/*
 * Hardware.hpp
 *
 *  Created on: 11 dec. 2018
 *      Author: Ola Andersson
 */

#ifndef HARDWARE_HPP_
#define HARDWARE_HPP_

#ifdef __AVR_ATmega328P__
	#include <avr/io.h>
//#include <avr/iom328p.h>
#endif /* __AVR_ATmega328P__ */

// Define for witch hardware to compile
#define SmedbyCharger11				1			// 5V version with Atmega 328P
#define SmedbyCharger11_33			0			// 3.3V version with Nano ARM, ATSAMD21
#define DCC							0			// 5V version with Atmel 328P

#define Serial						1
#define CAN							0

#if SmedbyCharger11 == 1

// Define what pins and ports to use on the MPU
// in reality, PD6 is really just '6'

	const int  InputVoltPin  		= PINC2;				// Pin used to measure Battery-volt PC2 = A2
	const int  InputVoltPort  		= 2;				// Port used to measure Battery-volt
	const int  OutputCurrentPin 	= PINC0;				// Pin used to measure Motor-volt PC0 = A0
	const int  OutputCurrentPort 	= 2;				// Port used to measure Motor-volt
	const int  OutputVoltPin 		= PINC1;				// Pin used  to measure output-volt PC1 = A1
	const int  OutputVoltPort 		= 2;				// Port used to measure output-volt
	const int  Pin12V 				= PINC3;				// Pin used to measure 12 volt rail PC3 = A3
	const int  Port12V 				= 2;				// Port used to measure 12 volt rail
	const int  TemperaturPin 		= PIND7;				// Pin used to measure mosfet-temperatur PD7 = D7
	const int  TemperaturPort 		= 3;				// Port used to measure mosfet-temperatur
	const int  pwmPin 				= PINB1;				// Pin used for PWM-charge-output PB1 = D9
	const int  pwmPort 				= 1;				// Port used for PWM-charge-output

	const int  Temp_Measure_type 	= 2;				// Type 1 = NTC-resistor, 2 = DS1820 temp sensor.
	const int  Current_Measure_type = 2;				// Type 1 = 5A, 2 = 20A, 3 = 30A (Defines witch type of current sense sensor is used.
	const int  RedLedPin	 		= PIND5;				// Red LED pin
	const int  RedLedPort 			= 3;				// Red LED port
	const int  YellowLedPin			= PIND4;				// Yellow LED pin
	const int  YellowLedPort		= 3;				// Yellow LED port
	const int  GreenLedPin 			= PIND3;				// Green LED pin
	const int  GreenLedPport		= 3;				// Green LED port
	const int  ButtonPin 			= PIND2;				// Button pin
	const int  ButtonPort 			= 3;				// Button port

#if Serial == 1
// Serialport settings
#define cBaudRate  115200
#endif // Serial = 1

#endif	// SmedbyCharger11 = 1

#if SmedbyCharger11_33 == 1

// Define what pins and ports to use on the MPU
// in reality, PD6 is really just '6'

	const int  InputVoltPin  		= PB09;				// Pin used to measure Battery-volt PC2 = A2
	const int  InputVoltPort  		= 2;				// Port used to measure Battery-volt
	const int  OutputCurrentPin 	= PA02;				// Pin used to measure Motor-volt PC0 = A0
	const int  OutputCurrentPort 	= 1;				// Port used to measure Motor-volt
	const int  OutputVoltPin 		= PB06;				// Pin used  to measure output-volt PC1 = A1
	const int  OutputVoltPort 		= 2;				// Port used to measure output-volt
	const int  Pin12V 				= PA04;				// Pin used to measure 12 volt rail PC3 = A3
	const int  Port12V 				= 1;				// Port used to measure 12 volt rail
	const int  TemperaturPin 		= PA21;				// Pin used to measure mosfet-temperatur PD7 = D7
	const int  TemperaturPort 		= 1;				// Port used to measure mosfet-temperatur
	const int  pwmPin 				= PA07;				// Pin used for PWM-charge-output PB1 = D9
	const int  pwmPort 				= 1;				// Port used for PWM-charge-output

	const int  Temp_Measure_type 	= 2;				// Type 1 = NTC-resistor, 2 = DS1820 temp sensor.
	const int  Current_Measure_type = 2;				// Type 1 = 5A, 2 = 20A, 3 = 30A (Defines witch type of current sense sensor is used.
	const int  RedLedPin	 		= PA15;				// Red LED pin
	const int  RedLedPort 			= 1;				// Red LED port
	const int  YellowLedPin			= PA08;				// Yellow LED pin
	const int  YellowLedPort		= 1;				// Yellow LED port
	const int  GreenLedPin 			= PA09;				// Green LED pin
	const int  GreenLedPport		= 1;				// Green LED port
	const int  ButtonPin 			= PA14;				// Button pin
	const int  ButtonPort 			= 1;				// Button port

#if Serial == 1
// Serialport settings
#define cBaudRate  38400
#endif // Serial = 1

#endif	// SmedbyCharger11_33


#if DCC == 1

// Define what pins and ports to use on the MPU
// in reality, PD6 is really just '6'

	const int  InputVoltPin  		= PC4;				// Pin used to measure inputVolt PC4 = A4
	const int  InputVoltPort  		= 2;				// Port used to measure inputvolt
	const int  OutputCurrentPin 	= PC1;				// Pin used to measure Output current PC1 = A1
	const int  OutputCurrentPort 	= 2;				// Port used to measure Output current
	const int  OutputVoltPin 		= PC2;				// Pin used  to measure output-volt PC2 = A2
	const int  OutputVoltPort 		= 2;				// Port used to measure output-volt
	const int  Pin12V 				= PC3;				// Pin used to measure 12 volt rail PC3 = A3
	const int  Port12V 				= 2;				// Port used to measure 12 volt rail
	const int  TemperaturPin 		= PD4;				// Pin used to measure mosfet-temperatur PD4 = D4
	const int  TemperaturPort 		= 3;				// Port used to measure mosfet-temperatur
	const int  pwmPin 				= PD6;				// Pin used for PWM-output PD6 = D6
	const int  pwmPort 				= 3;				// Port used for PWM-output

	const int  Temp_Measure_type 	= 2;				// Type 1 = NTC-resistor, 2 = DS1820 temp sensor.
	const int  Current_Measure_type = 3;				// Type 1 = 5A, 2 = 20A, 3 = 30A (Defines witch type of current sense sensor is used.
	const int  RedLedPin	 		= PB0;				// Red LED pin
	const int  RedLedPort 			= 1;				// Red LED port
	const int  YellowLedPin			= PB1;				// Yellow LED pin
	const int  YellowLedPort		= 1;				// Yellow LED port
	const int  GreenLedPin 			= PB2;				// Green LED pin
	const int  GreenLedPport		= 1;				// Green LED port
	const int  ButtonPin 			= PD5;				// Button pin
	const int  ButtonPort 			= 3;				// Button port

	const int  PotPin	 			= PD0;				// Potentiometer pin
	const int  PotPort 				= 2;				// Potentiometer port

#if Serial == 1
// Serialport settings
#define cBaudRate  115200
#endif // Serial = 1

#endif	// DCC = 1


#endif /* HARDWARE_HPP_ */
