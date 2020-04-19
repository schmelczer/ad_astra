#ifndef SLEEP_H
#define SLEEP_H

#include <stdbool.h>
#include <avr/io.h>


typedef bool (*FrameFunction)(uint8_t);

// frameFunction gets previousFrameTime (in milliseconds) as argument
void startFrameLoop(FrameFunction function, uint8_t frameLengthInMilliseconds);

#endif
