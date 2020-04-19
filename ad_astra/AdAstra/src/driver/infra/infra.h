#ifndef INFRA_H
#define INFRA_H

#include <stdbool.h>
#include <avr/io.h>


#define INFRA_ADDRESS 255
#define IR_PIN PB3
#define REPEAT_CODE 1

typedef void (*OnCommandReceived)(uint8_t);
typedef void (*OnReceiveStarted)();

void initializeInfra(OnCommandReceived onCommandReceived, OnReceiveStarted onReceiveStarted);

#endif
