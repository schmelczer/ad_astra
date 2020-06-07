#include "display.h"

#include <avr/pgmspace.h>
#include <avr/eeprom.h>

#include "bitwise.h"
#include "math.h"
#include "../../hardware_access/hardware_access.h"


static uint8_t const configuration[] PROGMEM = {
	0xD5, 0xF0,			/// set clock frequency
	0x8D, 0x14,			/// enable charge pump
	0x20, 0x00,			/// horizontal addressing mode
	0xD6, 0x01,			/// 2 times vertical zoom
	0x22, 0x00, 0x03,	/// only draw to the top half of the screen
	0xAF				/// display on
};

static struct {
	uint8_t compositingBuffer[DISPLAY_WIDTH_IN_PIXELS];
	Rectangle compositingWindow;
	DrawFunction drawEverything;
	bool wasIntersection;
} display;

static inline void commandMode() {
	setOutputPin(DISPLAY_DC_OUTPUT_PIN, false);
}

static inline void dataMode() {
	setOutputPin(DISPLAY_DC_OUTPUT_PIN, true);
}

void setDisplayContrast(uint8_t value) {
	commandMode();
	sendByteOnSPI(0x81);
	sendByteOnSPI(value);
	dataMode();
}

void initializeDisplay(DrawFunction drawEverything) {
	setOutputPin(DISPLAY_RESET_OUTPUT_PIN, false);
	for (volatile uint8_t i = 0; i != 255; i++)
		;
	/// some time has to elapse before the next line gets called,
	/// otherwise the display wont turn on
	setOutputPin(DISPLAY_RESET_OUTPUT_PIN, true);
	
	for (uint8_t i = 0; i < sizeof(configuration); i++) {
		sendByteOnSPI(pgm_read_byte(configuration + i));
	}
	
	display.drawEverything = drawEverything;
	setDisplayContrast(255);
	dataMode();
}

void startIntersectionTest(Rectangle compositingWindow) {
	display.wasIntersection = false;
	display.compositingWindow = compositingWindow;
}

bool endIntersectionTest() {
	for (uint8_t x = 0; x < DISPLAY_WIDTH_IN_PIXELS; x++) {
		display.compositingBuffer[x] = 0;
	}
	return display.wasIntersection;
}

void drawFrame() {
	display.compositingWindow = DEFAULT_COMPOSITING_WINDOW;
	for (display.compositingWindow.position.y = 0; display.compositingWindow.position.y < DISPLAY_HEIGHT_IN_PIXELS; display.compositingWindow.position.y += 8) {
		display.drawEverything(display.compositingWindow);

		for (uint8_t x = 0; x < DISPLAY_WIDTH_IN_PIXELS; x++) {
			sendByteOnSPI(display.compositingBuffer[x]);
			sendByteOnSPI(display.compositingBuffer[x]);
			display.compositingBuffer[x] = 0;
		}
	}
}

static void compositePixelColumn(uint8_t x, uint8_t invertedMask, uint8_t fill) {
	if (display.compositingBuffer[x] & fill) {
		display.wasIntersection = true;
	}
	display.compositingBuffer[x] = (display.compositingBuffer[x] & (~invertedMask)) | fill;
}

void drawBitmapFromProgMem(Rectangle boundingBox, uint16_t const bitmap[boundingBox.size.x][(boundingBox.size.y + 7) / 8], bool isMirrored) {
	boundingBox.position = substract(boundingBox.position, display.compositingWindow.position);

	uint8_t spriteY = max(0, -boundingBox.position.y);
	uint8_t spriteYByte = spriteY >> 3;

	for (uint8_t x = max(0, -boundingBox.position.x); x < boundingBox.size.x && boundingBox.position.x + x < DISPLAY_WIDTH_IN_PIXELS; x++) {
		uint8_t spriteX = isMirrored ? boundingBox.size.x - x - 1 : x;
		uint16_t currentPixelColumn = eeprom_read_word(&bitmap[spriteX][spriteYByte]);
		
		uint8_t fill, invertedMask;
		if (boundingBox.position.y >= 0) {
			fill = (currentPixelColumn & 0x00FF) << boundingBox.position.y;
			invertedMask = currentPixelColumn >> 8 << boundingBox.position.y;
		} else {
			uint16_t lowerPixelColumn = spriteYByte + 1 < (boundingBox.size.y + 7) / 8 ? eeprom_read_word(&bitmap[spriteX][spriteYByte + 1]) : 0;
			uint8_t shift = spriteY % 8;
			uint8_t inverseShift = 8 - shift;
			
			fill = ((currentPixelColumn & 0x00FF) >> shift) | ((lowerPixelColumn & 0x0FF) << inverseShift);
			invertedMask = (currentPixelColumn >> 8 >> shift) | (lowerPixelColumn >> 8 << inverseShift);
		}
		
		compositePixelColumn(boundingBox.position.x + x, invertedMask, fill);
	}
}

void drawFilledRectangle(Rectangle box, uint8_t invertedMask, uint8_t fill) {
	box.position = substract(box.position, display.compositingWindow.position);
	
	uint8_t upperGapHeight = min(8, max(0, box.position.y));
	uint8_t lowerGapHeight = min(8, max(0, 8 - (box.position.y + box.size.y)));
	
	uint8_t actualFill = (fill >> lowerGapHeight) & (fill << upperGapHeight);
	uint8_t actualInvertedMask = (invertedMask >> lowerGapHeight) & (invertedMask << upperGapHeight);

	for (uint8_t x = max(0, box.position.x); x < box.position.x + box.size.x && x < DISPLAY_WIDTH_IN_PIXELS; x++) {
		compositePixelColumn(x, actualInvertedMask, actualFill);
	}
}
