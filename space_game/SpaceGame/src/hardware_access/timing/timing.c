#include "timing.h"
#include "bitwise.h"


void enableTimerA(uint8_t triggerInterruptInXTicks) {
	OCR0A = TCNT0 + triggerInterruptInXTicks;
}

void enableTimerB(uint8_t triggerInterruptInXTicks) {
	setBit(TIFR, OCF0B);
	OCR0B = TCNT0 + triggerInterruptInXTicks;
	setBit(TIMSK, OCIE0B);
}

void disableTimerB() {
	clearBit(TIMSK, OCIE0B);
}

void enableFastTimerA(uint8_t triggerInterruptInXTicks) {
	setBit(TIFR, OCF1A);
	OCR1A = TCNT1 + triggerInterruptInXTicks;
	setBit(TIMSK, OCIE1A);
}

void disableFastTimerA() {
	clearBit(TIMSK, OCIE1A);
}

uint8_t getTimestampFromFastTimer() {
	return TCNT1;
}

uint8_t getTimeSince(uint8_t timestamp) {
	return timestamp <= TCNT1 ? 
		TCNT1 - timestamp :
		(uint8_t)(255 - timestamp) + TCNT1;
}
