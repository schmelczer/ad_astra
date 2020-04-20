#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include <avr/io.h>

#include "../../util/rectangle/rectangle.h"


/*
	SPI driver for D096-12864
*/

#define DISPLAY_RESET_OUTPUT_PIN PB0
#define DISPLAY_DC_OUTPUT_PIN PB4

// A two-times downscaling is used for greater performance
#define DISPLAY_WIDTH_IN_PIXELS 64
#define DISPLAY_HEIGHT_IN_PIXELS 32

// To easily access the window size
#define WINDOW ((Rectangle){(Vec2){0, 0}, (Vec2){DISPLAY_WIDTH_IN_PIXELS, DISPLAY_HEIGHT_IN_PIXELS}})
	
// To conserve RAM, drawing is done in chunks of DEFAULT_COMPOSITING_WINDOW size
// instead of buffering the whole display and writing it out at once
#define DEFAULT_COMPOSITING_WINDOW ((Rectangle){(Vec2){0, 0}, (Vec2){DISPLAY_WIDTH_IN_PIXELS, 8}})

typedef void (*DrawFunction)(Rectangle);

// Call DrawFunction n times after a call to drawFrame has been made
void initializeDisplay(DrawFunction drawEverything);

// Set a the brightness of the display
// Value can be any number between 0 and 255.
void setDisplayContrast(uint8_t value);

// To conserve program memory, pixel based intersection test
// is implemented here.
// Calling draw functions after calling startIntersectionTest
// will set a wasIntersection bit appropriately.

// Clear buffer and wasIntersection bit
void startIntersectionTest();

// Make the display go to / return from sleep
void turnDisplayOnOff(bool shouldBeOn);

// Return wasIntersection bit
bool endIntersectionTest();

// Initiate a draw sequence
void drawFrame();

// Draw a sprite of size boundingBox.size at boundingBox.position from bitmap
// if isMirrored then mirror around the vertical axis
// Bitmap's data is interpreted the following way:
/*
	Each 16 bit word corresponds to an 8 pixel high column.
	(These columns are laid out horizontally from left to right. Unfortunately,
	the display uses this addressing mode.) The higher 8 bits of the word is the 
	an inverted mask and the lower 8 bits are the fill bits.
	newPixelColumn = oldPixelColumn & ~invertedMask | fill;
	
	This seemingly weird layout is used to take advantage of SIMD operations
	and speed up the drawing process significantly.
*/
void drawBitmapFromProgMem(Rectangle boundingBox, uint16_t const bitmap[boundingBox.size.x][(boundingBox.size.y + 7) / 8], bool isMirrored);

// Draw a one byte repeated texture covering the parameter box
// for a white rectangle use these arguments: invertedMask: don't care, fill: 0xFF
// for a black rectangle use these arguments: invertedMask: 0xFF, fill: 0x00
void drawFilledRectangle(Rectangle box, uint8_t invertedMask, uint8_t fill);

#endif
