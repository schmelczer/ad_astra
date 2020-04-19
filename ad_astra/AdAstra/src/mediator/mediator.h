#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <avr/io.h>


// Setup the drivers, and business layer objects and their relations
// It is kind of a very basic dependency injection.
void setupConnections();

// Start drawing frames and ticking objects
void startGame();

// Make the machine go to sleep
void handleOff();

// Increase or decrease the contrast (brightness) of the display
// by the given value
// The contrast can be any number between 0 and 255.
// The function automatically clamps the contrast.
void changeDisplayContrast(int8_t by);

#endif
