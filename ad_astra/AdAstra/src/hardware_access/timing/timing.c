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
