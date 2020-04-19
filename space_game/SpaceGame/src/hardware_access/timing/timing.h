#ifndef TIMING_H
#define TIMING_H

#include <avr/io.h>
#include "bitwise.h"

inline void initializeTiming() {
	TCCR0B = BV(CS02);				 // CLK / 256
	TCCR1 = BV(CS13);	 // CLK / 128
	TIMSK = BV(OCIE0A) | BV(OCIE1A); // enable compare interrupts
}

#endif
