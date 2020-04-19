#ifndef RECEIVE_H
#define RECEIVE_H

// 8*10^6 / 256 / 1200
#define BIT_TIME 26
#define RECEIVE_PIN PB3


typedef void (*OnCommandReceived)(uint8_t);

void initializeUartReceive(OnCommandReceived onCommandReceived);

void handlePinChangeForUartReceive();

#endif
