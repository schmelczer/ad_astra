#ifndef SPI_H
#define SPI_H

#include <avr/io.h>
#include "bitwise.h"


#define DO_PIN		PB1
#define USCK_PIN	PB2

inline void initializeSPI() {
	DDRB |= BV(DO_PIN) | BV(USCK_PIN);	// set pin directions for MOSI and SCK
}

void sendByteOnSPI(uint8_t byte);

#endif
