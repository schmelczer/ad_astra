#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include <avr/io.h>

#include "../../util/rectangle/rectangle.h"


#define DISPLAY_RESET_OUTPUT_PIN PB0
#define DISPLAY_DC_OUTPUT_PIN PB4

#define DISPLAY_WIDTH_IN_PIXELS 64
#define DISPLAY_HEIGHT_IN_PIXELS 32

#define WINDOW ((Rectangle){(Vec2){0, 0}, (Vec2){DISPLAY_WIDTH_IN_PIXELS, DISPLAY_HEIGHT_IN_PIXELS}})
#define DEFAULT_COMPOSITING_WINDOW ((Rectangle){(Vec2){0, 0}, (Vec2){DISPLAY_WIDTH_IN_PIXELS, 8}})

typedef void (*DrawFunction)(Rectangle);

void initializeDisplay(DrawFunction drawEverything);
void setDisplayContrast(uint8_t value);

void turnDisplayOnOff(bool shouldBeOn);

void startIntersectionTest();
bool endIntersectionTest();

void drawFrame();
void drawBitmapFromProgMem(Rectangle boundingBox, uint16_t const data[boundingBox.size.x][(boundingBox.size.y + 7) / 8], bool isMirrored);

// for white rectangle: invertedMask: don't care, fill: 0xFF
// for black rectangle: invertedMask: 0xFF, fill: 0x00
void drawFilledRectangle(Rectangle box, uint8_t invertedMask, uint8_t fill);

#endif
