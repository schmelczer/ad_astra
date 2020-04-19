#ifndef ASTEROID_H
#define ASTEROID_H

#include <avr/io.h>
#include <stdbool.h>

#include "../../prototype.h"

#define ASTEROID_SIZE ((Vec2){8, 8})
#define IDLE_FRAME_COUNT 4
#define EXPLODING_FRAME_COUNT 3
#define EXPLODING_FRAME_CHANGE_INTERVAL 3

const Prototype Asteroid;

bool mineAsteroid(Object* asteroid);
bool isAsteroidIntersectingWithSpaceship(Object* asteroid, Object* spaceship);

struct _asteroid_t {
	bool isMirrored;
	uint8_t timeSinceLastFrameChange;
	uint8_t animationFrame;
};

#endif
