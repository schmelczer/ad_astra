#include "hardware_access.h"

#include <avr/interrupt.h>


void initializeHardwareAccess() {
	sei();
	initializePowerSaving();
	initializeSPI();
	initializeTiming();
	initializeOutputPins();
}
