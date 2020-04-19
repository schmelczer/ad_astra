#include "event_generator.h"

#include <avr/io.h>
#include <stdbool.h>

#include "null.h"

#include "../object.h"
#include "../object_container/object_container.h"
#include "../types/spaceship/spaceship.h"
#include "../types/background/background.h"
#include "../types/astronaut/astronaut.h"
#include "../types/asteroid/asteroid.h"
#include "../../util/random/random.h"
#include "../../driver/display/display.h"


typedef bool (*Predicate)(Rectangle*);


static inline Vec2 getRandomPosition() {
	return (Vec2) {
		getRandomNumber() % DISPLAY_WIDTH_IN_PIXELS,
		getRandomNumber() % DISPLAY_HEIGHT_IN_PIXELS
	};
}

static void generate(Prototype const* type, Predicate predicate) {
	Object* emptySpace = getEmptyObjectSpace();
	if (emptySpace == NULL || getRandomNumber() != 0) {
		return;
	}
	
	for (uint8_t tryCount = 0; tryCount < TRY_COUNT; tryCount++) {
		Rectangle proposedBoundingBox = (Rectangle){getRandomPosition(), getSizeFromPrototype(type)};
		
		if (predicate(&proposedBoundingBox)) {
			createObject(type, emptySpace);
			emptySpace->position = proposedBoundingBox.position;
			return;
		}
	}
}

bool generateAstronautPredicate(Rectangle* proposedBoundingBox) {
	return (
		(
			(getCountOf(&Astronaut) == 1 && spaceshipObject->as.spaceship.progress >= hasTable) ||
			(getCountOf(&Astronaut) == 2 && spaceshipObject->as.spaceship.progress >= hasFullCrew)
		) &&
		getIntersectingObjectOfType(*proposedBoundingBox, &Astronaut) == NULL &&
		isOnboard(*proposedBoundingBox)
	);
}

bool generateAsteroidPredicate(Rectangle* proposedBoundingBox) {
	return (
		getCountOf(&Asteroid) < MAX_ASTEROID_COUNT &&
		isInside(*proposedBoundingBox, WINDOW) &&
		getIntersectingObjectOfType(*proposedBoundingBox, &Spaceship) == NULL &&
		getIntersectingObjectOfType(*proposedBoundingBox, &Asteroid) == NULL
	);
}

void createPersistentObjects() {	
	createObject(&Spaceship, spaceshipObject);
	spaceshipObject->position = (Vec2){EXHAUST_BOUNDING_BOX.size.x, DISPLAY_HEIGHT_IN_PIXELS / 2 - getSize(spaceshipObject).y / 2};

	createObject(&Astronaut, character);
	Rectangle upperFloor = translateRectangle(UPPER_FLOOR_BOUNDING_BOX, spaceshipObject->position);
	character->position = add(upperFloor.position, (Vec2){10, 1});
}

void createTransientObjects() {
	createObject(&Background, getEmptyObjectSpace());
}

void generateEvents() {
	generate(&Astronaut, generateAstronautPredicate);
	generate(&Asteroid, generateAsteroidPredicate);
}
