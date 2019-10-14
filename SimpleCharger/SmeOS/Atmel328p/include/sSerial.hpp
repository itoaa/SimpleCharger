/*
 * sSerial.hpp
 *
 *  Created on: 10 nov. 2018
 *      Author: Ola Andersson
 */


#ifndef SMEOS_ATMEL328P_INCLUDE_SSERIAL_HPP_
#define SMEOS_ATMEL328P_INCLUDE_SSERIAL_HPP_

#define BAUD 115200
#define USE_2X					// Needed for 115200
#define BAUD_TOL 4				// Needed to not generate error when 115200 @ 16MHz

#include "avr/io.h"
#include <util/setbaud.h>
#include <stdio.h>

class sSerial
{
  private:
	void uartInit(void);
	static FILE uart_input;
	static FILE uart_output;

	//	int uartPutChar(char c, FILE *stream);
//	int uartGetChar(FILE *stream);

	FILE uart_str;
  public:
	sSerial(int baud);							// set baud rate
	sSerial();									// no constructor
	void	init(int baud);
	bool	dataRecived();
	void	putChar(uint8_t);					// Send one char to the serial
	uint8_t getChar();							// Get one char from the serial
};

#endif /* SMEOS_ATMEL328P_INCLUDE_SSERIAL_HPP_ */
