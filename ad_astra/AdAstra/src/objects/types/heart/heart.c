#include "heart.h"

#include <avr/pgmspace.h>

#include "../../../driver/display/display.h"
#include "../sprites.h"
#include "../../object.h"
#include "../../object_container/object_container.h"


static void tick(Object* heart, __attribute__((unused)) uint8_t previousFrameTime) {
	if (++heart->as.heart.timeSinceLastChange == HEART_ANIMATION_INTERVAL) {
		heart->as.heart.animationFrame = (heart->as.heart.animationFrame + 1) % HEART_FRAME_COUNT;
		heart->as.heart.timeSinceLastChange = 0;
	}
	if (++heart->as.heart.timeLived == TIME_TO_LIVE) {
		clearObject(heart);
	}
}

static void draw(Object* heart, Rectangle __attribute__((unused)) compositingWindow) {
	drawBitmapFromProgMem(
		getBoundingBox(heart),
		heart_blinking[heart->as.heart.animationFrame],
		false
	);
}

const Prototype Heart PROGMEM = {
	.tick = tick,
	.draw = draw,
	.size = HEART_SIZE,
};
