#ifndef HARDWARE_ACCESS_H
#define HARDWARE_ACCESS_H

#include <stdbool.h>
#include <avr/io.h>
#include "bitwise.h"

#include "power_saving/power_saving.h"
#include "spi/spi.h"
#include "eeprom/eeprom.h"
#include "timing/timing.h"
#include "output_pins/output_pins.h"


/*
	This module contains the lowest level functions to manipulate the hardware.
	You only have to include this header file which serves as a facade.
	The sub-modules' implementation can be freely changed as long as they
	still implement these functions.
*/

// Initialize every hardware element at once
void initializeHardwareAccess();

// Write a single byte, when finished call the callback method
// Writing of unchanged byte is atomic.
bool asyncWriteEEPROM(uint8_t* buffer, OnEEPROMFinished onFinished);

// Load byte from pointer
uint8_t loadByteEEPROM(uint8_t* address);

// Load 2 bytes from pointer
uint16_t loadWordEEPROM(uint16_t const* address);

// Load byte from address
// It is different from loadByteEEPROM because
// accepts a relative address instead of an
// absolute one.
uint8_t loadSavedByteEEPROM(uint8_t address);

// Enable the asynchronous writing of data from buffer
void enableWritingEEPROM();
void disableWritingEEPROM();

// Enable interrupt OCCRA for TIMER0 with a modulo of triggerInterruptInXTicks
void enableTimerA(uint8_t triggerInterruptInXTicks);

// Enable interrupt OCCRB for TIMER0B with a modulo of triggerInterruptInXTicks
void enableTimerB(uint8_t triggerInterruptInXTicks);
void disableTimerB();

// Send a single byte on the built-in SPI interface
// The transfer is done in a serial manner to achieve
// greater throughput.
void sendByteOnSPI(uint8_t byte);

// Set the value of an output pin
void setOutputPin(uint8_t id, bool value);

#endif
