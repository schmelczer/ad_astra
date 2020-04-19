#ifndef POWER_SAVING_H
#define POWER_SAVING_H

#include <avr/io.h>
#include "bitwise.h"


inline void initializePowerSaving() {
	setBit(ACSR, ACD);				// disable ADC to save power
	PRR = /*BV(PRTIM1) |*/ BV(PRADC);	// disable power of timer1 and ADC
}

#endif
