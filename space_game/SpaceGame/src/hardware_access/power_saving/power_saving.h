#ifndef POWER_SAVING_H
#define POWER_SAVING_H

#include <avr/io.h>
#include "bitwise.h"


inline void initializePowerSaving() {
	setBit(ACSR, ACD);	// disable ADC to save power
	PRR = BV(PRADC);	// disable power to ADC (again?)
}

#endif
