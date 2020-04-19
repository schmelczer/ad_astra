#include "asteroid.h"

#include <avr/pgmspace.h>

#include "../sprites.h"
#include "../../object.h"
#include "../../object_container/object_container.h"
#include "../../../util/vec2/vec2.h"
#include "../../../util/random/random.h"
#include "../../../driver/display/display.h"


bool mineAsteroid(Object* asteroid) {
	return ++asteroid->as.asteroid.animationFrame == IDLE_FRAME_COUNT;
}

static void tick(Object* asteroid, uint8_t previousFrameTime) {
	if (asteroid->as.asteroid.animationFrame < IDLE_FRAME_COUNT) {
		return;
	}
	
	if (++asteroid->as.asteroid.timeSinceLastFrameChange == EXPLODING_FRAME_CHANGE_INTERVAL) {
		if (++asteroid->as.asteroid.animationFrame >= IDLE_FRAME_COUNT + EXPLODING_FRAME_COUNT) {
			clearObject(asteroid);
		} else {
			asteroid->as.asteroid.timeSinceLastFrameChange = 0;
		}
	}
}

bool isAsteroidIntersectingWithSpaceship(Object* asteroid, Object* spaceship) {
	Rectangle bb = getBoundingBox(asteroid);
	startIntersectionTest(bb);
	drawObject(asteroid, bb);
	drawObject(spaceship, bb);
	return endIntersectionTest();
}

static void draw(Object* asteroid, Rectangle __attribute__((unused)) compositingWindow) {
	drawBitmapFromProgMem(
		getBoundingBox(asteroid),
		small_asteroid[asteroid->as.asteroid.animationFrame],
		asteroid->position.x % 2
	);
}

const Prototype Asteroid PROGMEM = {
	.tick = tick,
	.draw = draw,
	.size = ASTEROID_SIZE,
};
