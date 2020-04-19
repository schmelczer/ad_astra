#include "shift_register_output_pins.h"
#include "bitwise.h"
#include "../../spi/spi.h"

#include <stdbool.h>


static uint8_t currentValue = 0;

void sendCurrentValue() {
	clearBit(PORTB, CLK_ST_PIN);
	sendByteOnSPI(currentValue);
	setBit(PORTB, CLK_ST_PIN);
}

void setOutputPin(uint8_t id, bool value) {
	currentValue = (currentValue & ~BV(id)) | (value & 1) << id;
	sendCurrentValue();
}
