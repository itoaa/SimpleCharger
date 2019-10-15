/*
 * sSerial.cpp
 *
 *  Created on: 10 nov. 2018
 *      Author: Ola Andersson
 */

#include "sSerial.hpp"
extern "C" {

int uartPutChar(char c, FILE *stream) {
    if (c == '\n') {
        uartPutChar('\r', stream);
    }
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
    return 0;
}

int uartGetChar(FILE *stream) {
	loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
	return UDR0;
}
}

sSerial::sSerial(int baud)							// set baurrate
{
//	#define BAUD  baud;								// Set Baud rate
	uartInit();

	uart_str.put = uartPutChar;
	uart_str.get = uartGetChar;
	uart_str.flags = _FDEV_SETUP_RW;

	stdout = stdin = &uart_str;

//	uart_input = FDEV_SETUP_STREAM(NULL, uartGetChar, _FDEV_SETUP_READ);

};
sSerial::sSerial()							// set baurrate
{
//	#define BAUD  baud;								// Set Baud rate

//	uart_input = FDEV_SETUP_STREAM(NULL, uartGetChar, _FDEV_SETUP_READ);
};

void sSerial::init(int baud)
{
	uartInit();

	uart_str.put = uartPutChar;
	uart_str.get = uartGetChar;
	uart_str.flags = _FDEV_SETUP_RW;

	stdout = stdin = &uart_str;

}
void		sSerial::uartInit(void)
{
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
}

bool sSerial::dataRecived()
{
	if ((UCSR0A & (1 << RXC0)) == 0) return 0;
	else return 1;
}


void	sSerial::putChar(uint8_t c)					// Send one char to the serial
{
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
}
uint8_t sSerial::getChar()							// Get one char from the serial
{
	loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
	return UDR0;
}
