#include "astronaut.h"

#include <stdbool.h>
#include <avr/pgmspace.h>
#include "bitwise.h"
#include "../../../driver/display/display.h"
#include "../heart/heart.h"
#include "../../object_container/object_container.h"
#include "../sprites.h"
#include "../../../util/vec2/vec2.h"
#include "../../../util/random/random.h"
#include "../../object.h"


#define IS_MIRRORED_BIT 0
#define IS_CONTROLLING_SPACESHIP_BIT 1
#define WAS_DOING_ACTION_BIT 2

static inline bool getIsMirrored(Object* astronaut) {
	return getBit(astronaut->as.astronaut.flags, IS_MIRRORED_BIT);
}

static inline void setIsMirrored(Object* astronaut, bool value) {
	modifyBit(astronaut->as.astronaut.flags, IS_MIRRORED_BIT, value);
}

static inline bool getWasDoingAction(Object* astronaut) {
	return getBit(astronaut->as.astronaut.flags, WAS_DOING_ACTION_BIT);
}

static inline void setWasDoingAction(Object* astronaut, bool value) {
	modifyBit(astronaut->as.astronaut.flags, WAS_DOING_ACTION_BIT, value);
}


bool getIsControllingSpaceship(Object* astronaut) {
	return getBit(astronaut->as.astronaut.flags, IS_CONTROLLING_SPACESHIP_BIT);
}

static void setIsControllingSpaceship(Object* astronaut, bool value) {
	modifyBit(astronaut->as.astronaut.flags, IS_CONTROLLING_SPACESHIP_BIT, value);
}

static inline void applyGravity(Object* astronaut) {
	if (!isOnLadder(getBoundingBox(astronaut)) && !isBottomOnFloor(getBoundingBox(astronaut))) {
		move(astronaut, directions[south]);
	}
}

static void tick(Object* astronaut, uint8_t previousFrameTime) {
	if (astronaut->as.astronaut.timeSinceLastAction < TIME_BETWEEN_ACTION_CHANGE) {
		astronaut->as.astronaut.timeSinceLastAction += previousFrameTime;
	}
	
	applyGravity(astronaut);
}

void moveAstronaut(Object* astronaut, Vec2 direction) {
	if (
		   astronaut->as.astronaut.timeSinceLastAction < TIME_BETWEEN_ACTION_CHANGE
		&& !getWasDoingAction(astronaut)
	) {
		return;
	}
	astronaut->as.astronaut.timeSinceLastAction = 0;
	setWasDoingAction(astronaut, false);
	
	if (getIsControllingSpaceship(astronaut)) {
		moveSpaceship(direction);
	} else {
		Vec2 proposedPosition = add(astronaut->position, direction);
		Rectangle proposedBoundingBox = (Rectangle){proposedPosition, getSize(astronaut)};
		if (isOnboard(proposedBoundingBox)) {
			astronaut->position = proposedPosition;
			astronaut->as.astronaut.animationFrame = (astronaut->as.astronaut.animationFrame + 1) % MOVE_FRAME_COUNT;
		}
		
		if (direction.x == 0) {
			astronaut->as.astronaut.animationFrame = 0;
		}
		setIsMirrored(astronaut, direction.x < 0);
	}
}

void makeAstronautDoAction(Object* astronaut) {
	if (
		astronaut->as.astronaut.timeSinceLastAction < TIME_BETWEEN_ACTION_CHANGE
		&& getWasDoingAction(astronaut)
	) {
		return;
	}
	astronaut->as.astronaut.timeSinceLastAction = 0;
	setWasDoingAction(astronaut, true);
	
	if (getIsControllingSpaceship(astronaut)) {
		setIsControllingSpaceship(astronaut, false);
	} else {
		Object* heart;
		switch (getPossibleActionFromSpaceship(astronaut)) {
			case shootTurret:
				shootTurretOfSpaceship();
				break;
			case showLove:
				heart = getEmptyObjectSpace();
				if (heart != NULL) {
					for (uint8_t i = 0; i < OBJECT_COUNT; i++) {
						if (
						objects[i].prototype == &Astronaut &&
						areIntersecting(getBoundingBox(astronaut), getBoundingBox(objects + i)) &&
						objects + i != astronaut
						) {
							createObject(&Heart, heart);
							heart->position = add(astronaut->position, (Vec2){(getRandomNumber() % 11) - 5, -(getRandomNumber() % 5) - 10});
							break;
						}
					}
				}
				break;
			case repairingSpaceship:
				if (spaceshipObject->as.spaceship.healthLoss > 0) {
					spaceshipObject->as.spaceship.healthLoss--;
				}
				break;
			case controllingSpaceship:
				setIsControllingSpaceship(astronaut, true);
				break;
			default:
				break;
		}
	}
}

static void draw(Object* astronaut, __attribute__((unused)) Rectangle compositingWindow) {
	drawBitmapFromProgMem(
		getBoundingBox(astronaut),
		small_character_moving[astronaut->as.astronaut.animationFrame],
		getIsMirrored(astronaut)
	);
}

const Prototype Astronaut PROGMEM = {
	.tick = tick,
	.draw = draw,
	.size = ASTRONAUT_SIZE
};
