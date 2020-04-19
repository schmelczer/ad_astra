#ifndef SHIFT_REGITER_OUTPUT_PINS_H
#define SHIFT_REGITER_OUTPUT_PINS_H

#include <avr/io.h>

#include "bitwise.h"

#define CLK_ST_PIN PB0

// Uses a 74HC595 to extend the number of digital outputs
void sendCurrentValue();

inline void initializeOutputPins() {
	setBit(DDRB, CLK_ST_PIN);
	setBit(PORTB, CLK_ST_PIN);
	sendCurrentValue();
}

#endif
