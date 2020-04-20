#ifndef SLEEP_H
#define SLEEP_H

#include <stdbool.h>
#include <avr/io.h>

// FrameFunction gets previousFrameTime (in milliseconds) as argument
typedef bool (*FrameFunction)(uint8_t);

// Shut down the machine
void powerOff();

// Call function every frameLengthInMilliseconds while it returns true
void startFrameLoop(FrameFunction function, uint8_t frameLengthInMilliseconds);

#endif
