#include "bullet.h"

#include "../asteroid/asteroid.h"
#include "../spaceship/spaceship.h"
#include "../../object.h"
#include "../../object_container/object_container.h"
#include "null.h"
#include "../../../driver/display/display.h"



static void tick(Object* bullet, __attribute__((unused)) uint8_t previousFrameTime) {
	if (bullet->as.bullet.wereIntersectingInThePreviousFrame) {
		clearObject(bullet);
		return;
	}

	move(bullet, directions[east]);

	Object* asteroid = getIntersectingObjectOfType(getBoundingBox(bullet), &Asteroid);
	if (asteroid != NULL) {
		if (mineAsteroid(asteroid) || mineAsteroid(asteroid)) {
			onAsteroidMined();
		}
		bullet->as.bullet.wereIntersectingInThePreviousFrame = true;
	}
	
	if (!areIntersecting(getBoundingBox(bullet), WINDOW)) {
		bullet->as.bullet.wereIntersectingInThePreviousFrame = true;
	}
}

static void draw(Object* bullet, Rectangle __attribute__((unused)) compositingWindow) {
	drawFilledRectangle((Rectangle){bullet->position, BULLET_SIZE}, 0, 0xFF);
}

const Prototype Bullet PROGMEM = {
	.tick = tick,
	.draw = draw,
	.size = BULLET_SIZE,
};
