#include "spaceship.h"

#include <avr/pgmspace.h>

#include "../../object.h"
#include "../../../driver/display/display.h"
#include "../astronaut/astronaut.h"
#include "../asteroid/asteroid.h"
#include "../../object_container/object_container.h"
#include "../sprites.h"
#include "../../../util/vec2/vec2.h"
#include "../../../util/random/random.h"
#include "bitwise.h"


static uint8_t flickerState;

SpaceshipPart spaceshipParts[SPACESHIP_PART_COUNT] = {
	[TABLE_INDEX] = (SpaceshipPart) {
		{{7, 8}, {3, 3}},
		table[0],
		showLove,
		false
	},
	[BEDS_INDEX] = (SpaceshipPart) {
		{{3, 12}, {8, 6}},
		beds[0],
		repairingSpaceship,
		false
	},
	[COMMAND_PANEL_INDEX] = (SpaceshipPart) {
		{{26, 7}, {7, 4}},
		NULL,
		controllingSpaceship,
		true,
	},
	[TURRET_CONTROLLER_INDEX] = (SpaceshipPart) {
		{{26, 12}, {7, 6}},
		turret_controller[0],
		shootTurret,
		false
	}
};
	

bool isOnUpperFloor(Rectangle boundingBox) {
	return isInside(boundingBox, translateRectangle(UPPER_FLOOR_BOUNDING_BOX, spaceshipObject->position));
}

bool isOnLowerFloor(Rectangle boundingBox) {
	return isInside(boundingBox, translateRectangle(LOWER_FLOOR_BOUNDING_BOX, spaceshipObject->position));
}

bool isBottomOnFloor(Rectangle boundingBox) {
	return (
		add(spaceshipObject->position, UPPER_FLOOR_BOUNDING_BOX.position).y + UPPER_FLOOR_BOUNDING_BOX.size.y == boundingBox.position.y + boundingBox.size.y ||
		add(spaceshipObject->position, LOWER_FLOOR_BOUNDING_BOX.position).y + LOWER_FLOOR_BOUNDING_BOX.size.y == boundingBox.position.y + boundingBox.size.y
	);
}

bool isOnLadder(Rectangle boundingBox) {
	return areIntersecting(boundingBox, translateRectangle(LADDER_BOUNDING_BOX, spaceshipObject->position));
}

bool isOnboard(Rectangle boundingBox) {
	return isOnLowerFloor(boundingBox) || isOnUpperFloor(boundingBox) || isOnLadder(boundingBox);
}

Rectangle getBoundingBoxOfSpaceshipPart(SpaceshipPart* part) {
	return translateRectangle(part->boundingBox, spaceshipObject->position);
}

void shootTurretOfSpaceship() {
	Object* bullet = getEmptyObjectSpace();
	if (getEmptyObjectSpace() != NULL && spaceshipObject->as.spaceship.healthLoss < MAX_HEALTH - 1) {
		createObject(&Bullet, bullet);
		bullet->position = add(TURRET_POSITION, spaceshipObject->position);
		spaceshipObject->as.spaceship.healthLoss++;
	}
}

void onAsteroidMined() {
	switch (++spaceshipObject->as.spaceship.progress) {
		case hasBeds:
			setBit(spaceshipObject->as.spaceship.activatedParts, BEDS_INDEX);
			break;
		case hasTurret:
			setBit(spaceshipObject->as.spaceship.activatedParts, TURRET_CONTROLLER_INDEX);
			break;
		case hasTable:
			setBit(spaceshipObject->as.spaceship.activatedParts, TABLE_INDEX);
			break;
		default:
			break;
	}
}

void moveSpaceship(Vec2 direction) {
	Vec2 proposedPosition = add(spaceshipObject->position, direction);
	
	if (!isInside(translateRectangle(IN_VIEW_BOUNDING_BOX, proposedPosition), WINDOW)) {
		return;
	}
	
	for (uint8_t i = 0; i < OBJECT_COUNT; i++) {
		if (objects[i].prototype == &Astronaut) {
			move(objects + i, direction);
		}
	}

	move(spaceshipObject, direction);
	
	spaceshipObject->position = proposedPosition;

	for (uint8_t i = 0; i < OBJECT_COUNT; i++) {
		if (objects[i].prototype == &Asteroid && isAsteroidIntersectingWithSpaceship(objects + i, spaceshipObject)) {
			if (mineAsteroid(objects + i)) {
				spaceshipObject->as.spaceship.healthLoss += 2;
				onAsteroidMined();
			}
		}
	}
}

Action getPossibleActionFromSpaceship(Object* astronaut) {
	for (uint8_t i = 0; i < SPACESHIP_PART_COUNT; i++) {
		SpaceshipPart* part = spaceshipParts + i;
		if (
			isSpaceshipPartActivated(part) && areIntersecting(getBoundingBoxOfSpaceshipPart(part), getBoundingBox(astronaut))) {
			return part->possibleAction;
		}
	}
	
	return noAction;
}

void tick(Object* spaceship, uint8_t previousFrameTime) {
	flickerState = !flickerState;
	if (spaceship->as.spaceship.healthLoss >= MAX_HEALTH) {
		spaceship->as.spaceship.healthLoss++;
	}
}

bool isSpaceshipPartActivated(SpaceshipPart* part) {
	return part->alwaysActiveDoNotDraw || ((spaceshipObject->as.spaceship.activatedParts >> (part - spaceshipParts)) & 1);
}

static inline void drawSpaceshipHealthBar() {
	uint8_t actualBarLength = spaceshipObject->as.spaceship.healthLoss * BAR_LENGTH / MAX_HEALTH;
	drawFilledRectangle(
		(Rectangle){add(spaceshipObject->position, (Vec2){BAR_END_POSITION.x - actualBarLength, BAR_END_POSITION.y}), (Vec2){actualBarLength, 1}},
		0xFF, 0x00
	);
}

static inline void drawSpaceshipParts(Rectangle compositingWindow) {
	for (uint8_t i = 0; i < SPACESHIP_PART_COUNT; i++) {
		if (
			!(spaceshipParts + i)->alwaysActiveDoNotDraw && 
			isSpaceshipPartActivated(spaceshipParts + i) &&
			areIntersecting(compositingWindow, getBoundingBoxOfSpaceshipPart(spaceshipParts + i))
		) {
			drawBitmapFromProgMem(getBoundingBoxOfSpaceshipPart(spaceshipParts + i), spaceshipParts[i].sprite, false);
		}
	}
}

static inline void drawExhaust(Rectangle compositingWindow) {
	Rectangle exhaustRectangle = translateRectangle(EXHAUST_BOUNDING_BOX, spaceshipObject->position);
	if (
		areIntersecting(compositingWindow, exhaustRectangle) &&
		flickerState
	) {
		drawBitmapFromProgMem(exhaustRectangle, exhaust[0], false);
	}
}

static inline void drawGlitches() {
	for (uint8_t i = 0; i < spaceshipObject->as.spaceship.healthLoss - MAX_HEALTH; i++) {
		Rectangle r = translateRectangle((Rectangle){(Vec2){getRandomNumber() % SPACESHIP_SIZE.x, getRandomNumber() % SPACESHIP_SIZE.y}, (Vec2){8, 8}}, spaceshipObject->position);
		if (areIntersecting(r, WINDOW)) {
			drawFilledRectangle(r, 0xFF, 0x00);
		}
	}
}

static void draw(Object* spaceship, Rectangle compositingWindow) {
	drawBitmapFromProgMem(
		getBoundingBox(spaceship),
		spaceship_idle[0],
		false
	);
	
	drawSpaceshipParts(compositingWindow);
	drawSpaceshipHealthBar();
	
	if (spaceship->as.spaceship.healthLoss > MAX_HEALTH) {
		drawGlitches();
	} else {
		drawExhaust(compositingWindow);
	}
}

const Prototype Spaceship PROGMEM = {
	.tick = tick,
	.draw = draw,
	.size = SPACESHIP_SIZE,
};
