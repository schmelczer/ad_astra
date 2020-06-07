#ifndef INFRA_H
#define INFRA_H

#include <stdbool.h>
#include <avr/io.h>


/**
	Custom NEC implementation using a TSOP4838
*/

#define INFRA_ADDRESS 255
#define IR_PIN PB4
#define REPEAT_CODE 1

typedef void (*OnCommandReceived)(uint8_t);

/// Initialize infra and call onCommandReceived with every received byte
/// Call onCommandReceived with the argument REPEAT_CODE if a repeat code 
/// has been received.
void initializeInfra(OnCommandReceived onCommandReceived);

#endif
