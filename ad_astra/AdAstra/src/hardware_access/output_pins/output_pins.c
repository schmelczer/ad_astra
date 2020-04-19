#include "output_pins.h"
#include "bitwise.h"

#include <avr/io.h>
#include <stdbool.h>


void setOutputPin(uint8_t id, bool value) {
	setBit(DDRB, id);
	modifyBit(PORTB, id, value); 
}
