#ifndef RECEIVE_H
#define RECEIVE_H

/* 
	Custom UART receive implementation with a baud rate of 2400
*/


// 8*10^6 / 128 / 2400
#define BIT_TIME 26
#define RECEIVE_PIN PB3

typedef void (*OnCommandReceived)(uint8_t);

// Initialize UART and call onCommandReceived with every byte received
void initializeUartReceive(OnCommandReceived onCommandReceived);

// Check for change on the UART receive pin
// This function exist so we can use a single interrupt 
// handler for both the UART receive and infra pins.
// Unfortunately, it's not possible to use more than one handler.
void handlePinChangeForUartReceive();

#endif
