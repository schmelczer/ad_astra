#include "background.h"

#include <stdbool.h>
#include <avr/pgmspace.h>

#include "../../object.h"
#include "../../../util/rectangle/rectangle.h"
#include "../../../util/random/random.h"
#include "../sprites.h"
#include "../../../driver/display/display.h"


typedef struct {
	Vec2 position;
	uint8_t type;
} Star;

static Star backgroundStars[STAR_COUNT];

static Star createStarOnTheRight() {
	return (Star){
		(Vec2){DISPLAY_WIDTH_IN_PIXELS, getRandomNumber() % (DISPLAY_HEIGHT_IN_PIXELS - STAR_SIZE)},
		getRandomNumber() % STAR_SHAPE_COUNT
	};
}

static void tick(Object* background, uint8_t previousFrameTime) {
	background->as.background.movementState++;
	for (uint8_t i = 0; i < STAR_COUNT; i++) {
		switch (i % 3) {
			case 0:
				backgroundStars[i].position.x -= 1;
				break;
			case 1:
				backgroundStars[i].position.x -= background->as.background.movementState & 1;
				break;
			case 2:
				backgroundStars[i].position.x -= ~background->as.background.movementState & 1;
				break;
		}
		if (backgroundStars[i].position.x == -STAR_SIZE) {
			backgroundStars[i] = createStarOnTheRight();
		} 
	}
}

static void draw(__attribute__((unused)) Object* background, Rectangle compositingWindow) {
	for (uint8_t i = 0; i < STAR_COUNT; i++) {
		Rectangle starBoundingBox = (Rectangle){backgroundStars[i].position, (Vec2){STAR_SIZE, STAR_SIZE}};
		if (areIntersecting(compositingWindow, starBoundingBox)) {
			drawBitmapFromProgMem(
				starBoundingBox,
				stars[backgroundStars[i].type],
				false
			);
		}
	}
}

void initializeBackground() {
	for (uint8_t i = 0; i < STAR_COUNT; i++) {
		backgroundStars[i] = createStarOnTheRight();
		backgroundStars[i].position.x = getRandomNumber();
	}
}

const Prototype Background PROGMEM = {
	.tick = tick,
	.draw = draw,
	.size = (Vec2){DISPLAY_WIDTH_IN_PIXELS, DISPLAY_HEIGHT_IN_PIXELS}	/// == WINDOW.size
};
