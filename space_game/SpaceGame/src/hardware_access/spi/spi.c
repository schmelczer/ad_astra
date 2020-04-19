#include "spi.h"

#include <avr/interrupt.h>

void sendByteOnSPI(uint8_t byte) {
	cli();
	USIDR = byte;
	for (uint8_t i = 16; i != 0; i--) {
		USICR = BV(USIWM0) | BV(USICLK) | BV(USITC) | BV(USICS1);
	}
	sei();
}
