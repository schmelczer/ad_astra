#ifndef TRANSMIT_H
#define TRANSMIT_H

#include <avr/io.h>


// 8*10^6 / 256 / 1200
#define BIT_TIME 26
#define ASYNC_BUFFER_SIZE 20
#define UART_OUTPUT_PIN 2


void initializeUartTransmit();
void sendByteOnUartAsync(char byte);
void sendTextOnUartAsync(char text[]);
void sendUintOnUartAsync(uint8_t number);

#endif
