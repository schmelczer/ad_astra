#ifndef TIMING_H
#define TIMING_H

#include <avr/io.h>
#include "bitwise.h"


inline void initializeTiming() {
	TCCR0B = BV(CS02);		// CLK / 256
	setBit(TIMSK, OCIE0A);
}

#endif
