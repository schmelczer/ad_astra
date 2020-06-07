#include <stdbool.h>
#include <avr/interrupt.h>

#include "receive.h"
#include "bitwise.h"
#include "../../hardware_access/hardware_access.h"


static struct {
	uint16_t message;
	uint8_t bitPosition;		/// goes from 0 to 9
	bool previousValue;
	uint8_t previousTimestamp;
	OnCommandReceived onCommandReceived;
} uartReceive = {
	.previousValue = true	
};

void initializeUartReceive(OnCommandReceived onCommandReceived) {
	setBit(PORTB, RECEIVE_PIN);		/// setup pull-up
	setBit(PCMSK, RECEIVE_PIN);		/// enable interrupt on pin
	uartReceive.onCommandReceived = onCommandReceived;
}

void handlePinChangeForUartReceive() {
	bool currentValue = getBit(PINB, RECEIVE_PIN);
	
	if (uartReceive.previousValue != currentValue) {
		uint8_t elapsedBitCount = (getTimeSince(uartReceive.previousTimestamp) + BIT_TIME / 2) / BIT_TIME;
		uartReceive.message >>= elapsedBitCount;
		uartReceive.bitPosition += elapsedBitCount;
		
		if (uartReceive.bitPosition == 9) {
			uartReceive.onCommandReceived((uint8_t)(uartReceive.message));
			uartReceive.bitPosition = 0;
		}

		if (uartReceive.previousValue == true && currentValue == false) {
			if (uartReceive.bitPosition != elapsedBitCount) {
				uartReceive.message |= (uint8_t)(0xFF << (8 - elapsedBitCount));
			} else {
				uartReceive.bitPosition = 0;
			}
		}
		
		uartReceive.previousTimestamp = getTimestampFromFastTimer();
		uartReceive.previousValue = currentValue;
	}
}
