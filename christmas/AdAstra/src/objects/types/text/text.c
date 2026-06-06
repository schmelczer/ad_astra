#include "text.h"

#include <avr/eeprom.h>

#include "../asteroid/asteroid.h"
#include "../spaceship/spaceship.h"
#include "../sprites.h"
#include "../../object.h"
#include "../../object_container/object_container.h"
#include "null.h"
#include "math.h"
#include "../../../driver/display/display.h"

#include <stdbool.h>
#define STRING_TERMINATOR 255
#define CHARACTER_WIDTH 4

static bool canCreateNext = true;
static int8_t previousTextIndex = -1;
static bool isSpedUp = false;

bool createText(Object* object) {
	if (!canCreateNext) {
		return false;
	}
	
	createObject(&Text, object);
	object->as.text.xPosition = DISPLAY_WIDTH_IN_PIXELS;
	object->as.text.hasSetCanCreate = false;
	object->position = (Vec2){0, 0};
	previousTextIndex++;
	isSpedUp = false;
	uint8_t textCount = 0;
	for (uint16_t i = 0; i < 512; i++) {
		if (textCount == previousTextIndex) {
			if (eeprom_read_byte(i) == STRING_TERMINATOR) {
				previousTextIndex = 0;
				object->as.text.textStartIndex = 0;
			} else {
				object->as.text.textStartIndex = i;
			}
			canCreateNext = false;
			return;
		}
		
		if (eeprom_read_byte(i) == STRING_TERMINATOR) {
			textCount++;
		}
	}
	
	return true;
}

void speedUpText() {
	isSpedUp = true;
}

static uint8_t getTextLength(Object* text) {
	for (uint8_t i = 0; i < 255; i++) {
		if (eeprom_read_byte(text->as.text.textStartIndex + i) == STRING_TERMINATOR) {
			return i;
		}
	}
	
	return 0;
}

static void tick(Object* text, __attribute__((unused)) uint8_t previousFrameTime) {
	if (++text->as.text.timeSinceLastShift > 2 || isSpedUp) {
		text->as.text.xPosition--;
		text->as.text.timeSinceLastShift = 0;
	}
	
	if (
		   !text->as.text.hasSetCanCreate
		&& getTextLength(text) * (CHARACTER_WIDTH + 1) + text->as.text.xPosition < DISPLAY_WIDTH_IN_PIXELS - CHARACTER_WIDTH - 1
	) {
		canCreateNext = true;
		text->as.text.hasSetCanCreate = true;
	}
	
	if (getTextLength(text) * (CHARACTER_WIDTH + 1) < -text->as.text.xPosition) {
		clearObject(text);
	}
}

static void draw(Object* text, Rectangle __attribute__((unused)) compositingWindow) {
	uint8_t indexOfFirstVisible = max(0, -text->as.text.xPosition / (CHARACTER_WIDTH + 1));
	for (uint8_t i = indexOfFirstVisible; i < indexOfFirstVisible + DISPLAY_WIDTH_IN_PIXELS / (CHARACTER_WIDTH + 1) + 2; i++) {
		uint8_t letter = eeprom_read_byte(text->as.text.textStartIndex + i);
		if (letter == STRING_TERMINATOR) {
			return;
		}
		
		drawBitmapFromProgMem(
			(Rectangle){(Vec2){text->as.text.xPosition + i * (CHARACTER_WIDTH + 1), 0}, (Vec2){4, 8}},
			font[letter],
			false
		);
		
		drawFilledRectangle(
			(Rectangle){(Vec2){text->as.text.xPosition + i * (CHARACTER_WIDTH + 1) + CHARACTER_WIDTH, 0},
			(Vec2){1, 8}},
			0xFF,
			0x00
		);
	}
}

const Prototype Text PROGMEM = {
	.tick = tick,
	.draw = draw,
	.size = TEXT_SIZE,
};
