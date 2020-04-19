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

void enableTimerA(uint8_t triggerInterruptInXTicks);
void enableTimerB(uint8_t triggerInterruptInXTicks);
void disableTimerB();
void enableFastTimerA(uint8_t triggerInterruptInXTicks);
void disableFastTimerA();
uint8_t getTimestampFromFastTimer();
uint8_t getTimeSince(uint8_t timestamp);

void sendByteOnSPI(uint8_t byte);

void setOutputPin(uint8_t id, bool value);

#endif