#ifndef SPACESHIP_H
#define SPACESHIP_H


#include "../../prototype.h"
#include "../../../util/rectangle/rectangle.h"
#include "../astronaut/astronaut.h"

#include <stdbool.h>

#define SPACESHIP_SIZE ((Vec2){36, 23})

#define IN_VIEW_BOUNDING_BOX ((Rectangle){(Vec2){7, 4}, (Vec2){22, 15}})
#define UPPER_FLOOR_BOUNDING_BOX ((Rectangle){(Vec2){8, 5}, (Vec2){19, 6}})
#define LOWER_FLOOR_BOUNDING_BOX ((Rectangle){(Vec2){5, 12}, (Vec2){23, 6}})
#define EXHAUST_BOUNDING_BOX ((Rectangle){(Vec2){-4, 9}, (Vec2){5, 5}})

#define TURRET_POSITION ((Vec2){35, 11})

#define LADDER_BOUNDING_BOX ((Rectangle){(Vec2){12, 10}, (Vec2){1, 4}})

#define BOBBING_INTERVAL 130
#define SPACESHIP_PART_COUNT 4

#define TABLE_INDEX 3
#define BEDS_INDEX 2
#define COMMAND_PANEL_INDEX 1
#define TURRET_CONTROLLER_INDEX 0

#define BAR_END_POSITION ((Vec2){33, 11})
#define BAR_LENGTH 4
#define MAX_HEALTH 8

typedef struct {
	Rectangle boundingBox;
	uint16_t** sprite;
	Action possibleAction;
	bool alwaysActiveDoNotDraw;
} SpaceshipPart;

SpaceshipPart spaceshipParts[SPACESHIP_PART_COUNT];

const Prototype Spaceship;

typedef enum {
	hasBeds = 2,
	hasTurret = 5,
	hasHalfCrew = 8,
	hasTable = 15
} Progress;

struct _spaceship_t {
	uint8_t healthLoss;
	uint8_t progress;
	uint8_t activatedParts;
};

bool isOnboard(Rectangle boundingBox);
void moveSpaceship(Vec2 direction);
Rectangle getBoundingBoxOfSpaceshipPart(SpaceshipPart* part);

bool isBottomOnFloor(Rectangle boundingBox);
bool isOnUpperFloor(Rectangle boundingBox);
bool isOnLowerFloor(Rectangle boundingBox);
bool isOnLadder(Rectangle boundingBox);
void onAsteroidMined();
bool isSpaceshipPartActivated(SpaceshipPart* part);

bool isSpaceshipDestroyed();
void destroySpaceship();

void shootTurretOfSpaceship(); 
Action getPossibleActionFromSpaceship(Object* astronaut);

#endif
