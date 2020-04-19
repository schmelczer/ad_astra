#include "sleep.h"

#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "bitwise.h"
#include "../../hardware_access/hardware_access.h"


#define TICKS_IN_MILISECOND 31

volatile int8_t milisecondsSinceFrameStart;

void startFrameLoop(FrameFunction function, uint8_t frameLengthInMilliseconds) {
	sleep_enable();
	
	uint8_t previousFrameTime = 0;
	while (function(previousFrameTime)) {
		previousFrameTime = milisecondsSinceFrameStart;
		
		while (milisecondsSinceFrameStart < frameLengthInMilliseconds) {
			clearBit(MCUCR, SM1);	// idle mode
			sleep_cpu();
		}
		
		milisecondsSinceFrameStart = 0;
	}
}

void powerOff() {
	setBit(MCUCR, SM1);		// power-down mode
	sleep_cpu();
}

ISR(TIM0_COMPA_vect) {
	milisecondsSinceFrameStart++;
	enableTimerA(TICKS_IN_MILISECOND);
}