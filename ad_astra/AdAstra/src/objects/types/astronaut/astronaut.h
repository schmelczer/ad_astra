#ifndef ASTRONAUT_H
#define ASTRONAUT_H

#include "../../prototype.h"
#include <stdbool.h>


#define ASTRONAUT_SIZE ((Vec2){5, 5})
#define MOVE_FRAME_COUNT 4

// Between two consecutive actions (or movements)
// there has to be at least this many milliseconds
#define TIME_BETWEEN_ACTION_CHANGE 40

typedef enum {
	noAction = 0,
	controllingSpaceship,
	shootTurret,
	showLove,
	repairingSpaceship,
	ACTION_COUNT
} Action;


const Prototype Astronaut;

struct _astronaut_t {
	uint8_t flags;
	uint8_t animationFrame;
	uint8_t timeSinceLastAction;
};

void moveAstronaut(Object* astronaut, Vec2 unitVector);
void makeAstronautDoAction(Object* astronaut);
bool getIsControllingSpaceship(Object* astronaut);

#endif
