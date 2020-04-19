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


void initializeHardwareAccess();

// writing of unchanged byte is atomic
bool asyncWriteEEPROM(uint8_t* buffer, OnEEPROMFinished onFinished);
uint8_t loadSavedByteEEPROM(uint8_t address);
uint8_t loadByteEEPROM(uint8_t* address);
uint16_t loadWordEEPROM(uint16_t const* address);
void enableWritingEEPROM();
void disableWritingEEPROM();

void enableTimerA(uint8_t triggerInterruptInXTicks);
void enableTimerB(uint8_t triggerInterruptInXTicks);
void disableTimerB();

void sendByteOnSPI(uint8_t byte);

void setOutputPin(uint8_t id, bool value);

#endif