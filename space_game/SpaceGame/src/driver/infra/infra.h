#ifndef INFRA_H
#define INFRA_H

#include <stdbool.h>
#include <avr/io.h>


#define INFRA_ADDRESS 255
#define IR_PIN PB4
#define REPEAT_CODE 1

typedef void (*OnCommandReceived)(uint8_t);

void initializeInfra(OnCommandReceived onCommandReceived);

#endif
