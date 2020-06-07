#include "ai.h"

#include <stdbool.h>
#include <avr/io.h>

#include "../object_container/object_container.h"
#include "../types/astronaut/astronaut.h"
#include "../types/spaceship/spaceship.h"
#include "../../util/rectangle/rectangle.h"
#include "../../util/random/random.h"
#include "../../driver/display/display.h"


#define AI_ACTION_COUNT 5

typedef bool (*Predicate)(Rectangle*, Object*);
typedef void (*Execution)(Object*);

static uint8_t timeSinceLastAction;


typedef struct {
	Predicate predicate;
	Execution execution;
	SpaceshipPart* spaceshipPart;
	bool onlyOneAstronautCanDoIt;
	Vec2 deltaCenter;
	bool isSomeoneDoingThis;
} AIAction;

static AIAction actions[AI_ACTION_COUNT];


static Vec2 whichDirectionToMove(Object* astronaut, Vec2 position) {
	bool const isTargetOnUpperFloor = isOnUpperFloor((Rectangle){position, (Vec2){0, 0}});		/// else it's on the lower floor

	Vec2 const ladder = add(LADDER_BOUNDING_BOX.position, spaceshipObject->position);
	
	Vec2 astronautCenter = getCenter(getBoundingBox(astronaut));
	Vec2 target = astronautCenter;
	 if ((
			isOnUpperFloor(getBoundingBox(astronaut)) && isTargetOnUpperFloor
		) || (
			isOnLowerFloor(getBoundingBox(astronaut)) && !isTargetOnUpperFloor
		)) {
		target.x = position.x;
	} else if (isOnLadder(getBoundingBox(astronaut))){
		target.y = add(position, (Vec2){0, isTargetOnUpperFloor ? -10 : 10}).y;
	} else {
		target.x = ladder.x;
	}
	
	return clampVec2(substract(target, astronautCenter));
}

static void carefullyMoveAstronaut(Object* astronaut, Vec2 target) {
	if (getIsControllingSpaceship(astronaut)) {
		makeAstronautDoAction(astronaut);
	}
	if (!getIsControllingSpaceship(astronaut)) {
		moveAstronaut(astronaut, whichDirectionToMove(astronaut, target));
	}
}

static void carefullyMoveSpaceship(Object* astronaut, Vec2 target) {
	if (!getIsControllingSpaceship(astronaut)) {
		makeAstronautDoAction(astronaut);
	}
	if (getIsControllingSpaceship(astronaut)) {
		Vec2 direction = clampVec2(
			substract(
				target, 
				getCenter(getBoundingBox(spaceshipObject))
			)
		);
		
		moveAstronaut(astronaut, direction);
	}
}

static void makeAiAstronautDoAction(Object* astronaut) {
	timeSinceLastAction = 0;
	makeAstronautDoAction(astronaut);
}

static bool shouldControlTurret(
	__attribute__((unused)) Rectangle* boundingBox,
	__attribute__((unused)) Object* astronaut
) {
	return getCountOf(&Asteroid) > 0;
}

static void executeControlTurret(Object* astronaut) {
	if (getIntersectingObjectOfType(
		(Rectangle){
			add(TURRET_POSITION, spaceshipObject->position),
			(Vec2){63, 1}
		},
		&Asteroid
	)) {
		makeAiAstronautDoAction(astronaut);
	};
}

static bool shouldControlSpaceship(
	__attribute__((unused)) Rectangle* boundingBox, 
	__attribute__((unused)) Object* astronaut
) {
	return    getCountOf(&Asteroid) > 0 
		   && spaceshipObject->as.spaceship.healthLoss < MAX_HEALTH / 4 * 3;
}

static void executeControlSpaceship(Object* astronaut) {
	carefullyMoveSpaceship(
		astronaut, 
		add(
			getCenter(getBoundingBox(getFirstOfType(&Asteroid))),
			(Vec2){-30, 0}
		)
	);
}

static bool shouldRepairSpaceship(Rectangle* boundingBox, Object* astronaut) {
	return (
		(
			areIntersecting(*boundingBox, getBoundingBox(astronaut)) &&
			spaceshipObject->as.spaceship.healthLoss > 0
		) ||
			spaceshipObject->as.spaceship.healthLoss >= MAX_HEALTH / 2
	);
}

static bool shouldCenterSpaceship(Rectangle* boundingBox, Object* astronaut) {
	return (
		!actions[0].isSomeoneDoingThis && 
		(
			areIntersecting(*boundingBox, getBoundingBox(astronaut)) ||
			getCenter(getBoundingBox(spaceshipObject)).x != getCenter(WINDOW).x ||
			getCenter(getBoundingBox(spaceshipObject)).y != getCenter(WINDOW).y
		)
	);
}

static void executeCenterSpaceship(Object* astronaut) {
	carefullyMoveSpaceship(
		astronaut,
		getCenter(WINDOW)
	);
}

static bool shouldSocialize(
	__attribute__((unused)) Rectangle* boundingBox, 
	__attribute__((unused)) Object* astronaut
) {
	return true;
}

static void executeSocialize(Object* astronaut) {
	if (getRandomNumber() % 150 == 0) {
		makeAstronautDoAction(astronaut);
	}
}

static AIAction actions[AI_ACTION_COUNT] = {
	(AIAction) {
		.predicate = shouldControlSpaceship,
		.execution = executeControlSpaceship,
		.spaceshipPart = spaceshipParts + COMMAND_PANEL_INDEX,
		.onlyOneAstronautCanDoIt = true,
		.deltaCenter = {-3, 0}
	},
	(AIAction) {
		.predicate = shouldRepairSpaceship,
		.execution = makeAiAstronautDoAction,
		.spaceshipPart = spaceshipParts + BEDS_INDEX,
		.onlyOneAstronautCanDoIt = true,
		.deltaCenter = {2, 0}
	},
	(AIAction) {
		.predicate = shouldControlTurret,
		.execution = executeControlTurret,
		.spaceshipPart = spaceshipParts + TURRET_CONTROLLER_INDEX,
		.onlyOneAstronautCanDoIt = true,
		.deltaCenter = {-3, 0}
	},
	(AIAction) {
		.predicate = shouldCenterSpaceship,
		.execution = executeCenterSpaceship,
		.spaceshipPart = spaceshipParts + COMMAND_PANEL_INDEX,
		.onlyOneAstronautCanDoIt = true,
		.deltaCenter = {-3, 0}
	},
	(AIAction) {
		.predicate = shouldSocialize,
		.execution = executeSocialize,
		.spaceshipPart = spaceshipParts + TABLE_INDEX,
		.onlyOneAstronautCanDoIt = false,
		.deltaCenter = {2, 0}		
	},
};

void handleAI() {
	timeSinceLastAction++;
	
	for (uint8_t j = 0; j < ACTION_COUNT; j++) {
		actions[j].isSomeoneDoingThis = false;
	}
	
	for (uint8_t i = 0; i < OBJECT_COUNT; i++) {
		if (objects[i].prototype == &Astronaut && objects + i != character) {
			for (uint8_t j = 0; j < ACTION_COUNT; j++) {
				AIAction* currentAction = actions + j;
				Rectangle boundingBox = getBoundingBoxOfSpaceshipPart(currentAction->spaceshipPart);
				Object* astronautIntersectingBoundingBox = getIntersectingObjectOfType(boundingBox, &Astronaut);
				if (
					isSpaceshipPartActivated(currentAction->spaceshipPart) &&
					(!currentAction->onlyOneAstronautCanDoIt || (!currentAction->isSomeoneDoingThis && astronautIntersectingBoundingBox != character)) &&
					 currentAction->predicate(&boundingBox, objects + i)
				) {
					if (!areIntersecting(boundingBox, getBoundingBox(objects + i))) {
						carefullyMoveAstronaut(objects + i, add(getCenter(boundingBox), currentAction->deltaCenter));
					} else if (timeSinceLastAction > AI_ACTION_INTERVAL) {
						currentAction->execution(objects + i);
					}
					currentAction->isSomeoneDoingThis = true;
					break;
				}
			}
		}
	}
}
