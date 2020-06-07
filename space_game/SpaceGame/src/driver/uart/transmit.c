#include "transmit.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "bitwise.h"
#include "../../hardware_access/hardware_access.h"

static struct {
	char bytes[ASYNC_BUFFER_SIZE];
	uint8_t start;
	uint8_t end;
	/// 0 - start bit, 1-8 word bits, 9 - end bit 
	uint8_t currentMaskPosition;
} uartTransmitter;

static inline void send0() {
	setOutputPin(UART_OUTPUT_PIN, false);
}

static inline void send1() {
	setOutputPin(UART_OUTPUT_PIN, true);
}

ISR(TIM1_COMPA_vect) {
	enableFastTimerA(BIT_TIME);
	switch(uartTransmitter.currentMaskPosition) {
		case 0:
			send0();
			uartTransmitter.currentMaskPosition++;
			break;
		case 9:
			send1();
			uartTransmitter.start = (uartTransmitter.start + 1) % ASYNC_BUFFER_SIZE;
			if (uartTransmitter.start == uartTransmitter.end) {
				disableFastTimerA();
			}
			uartTransmitter.currentMaskPosition = 0;
			break;
		default:
			uartTransmitter.bytes[uartTransmitter.start] & BV(uartTransmitter.currentMaskPosition - 1) ? send1() : send0();
			uartTransmitter.currentMaskPosition++;
	}
}

void sendByteOnUartAsync(char byte) {
	cli();
	uartTransmitter.bytes[uartTransmitter.end] = byte;
	if (uartTransmitter.start == uartTransmitter.end) {
		enableFastTimerA(BIT_TIME);
	}
	uartTransmitter.end = (uartTransmitter.end + 1) % ASYNC_BUFFER_SIZE;
	
	sei();
}

void sendTextOnUartAsync(char text[]) {
	for (uint8_t i = 0; text[i]; i++) {
		sendByteOnUartAsync(text[i]);
	}
}

void sendUintOnUartAsync(uint8_t number) {
	sendByteOnUartAsync(number >= 100 ? '0' + number / 100 : ' ');
	number %= 100;
	sendByteOnUartAsync(number >= 10 ? '0' + number / 10 : ' ');
	number %= 10;
	sendByteOnUartAsync('0' + number);
}


void initializeUartTransmit() {
	send1();
}
