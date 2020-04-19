#ifndef TRANSMIT_H
#define TRANSMIT_H

#include <avr/io.h>


/* 
	Custom UART trasnmit implementation with a baud rate of 2400
*/

// 8*10^6 / 128 / 2400
#define BIT_TIME 26

// Messages cannot be longer than this many characters (including trailing zero)
#define ASYNC_BUFFER_SIZE 20

#define UART_OUTPUT_PIN 2

// Start pulling-up the transmit line
void initializeUartTransmit();

// Send a single byte over on UART
// It's done in an asynchronous fashion using a circular buffer.
void sendByteOnUartAsync(char byte);

// Send a string over on UART
// It's done in an asynchronous fashion using a circular buffer.
void sendTextOnUartAsync(char text[]);

// Send the decimal form of unsigned integer over UART
void sendUintOnUartAsync(uint8_t number);

#endif
