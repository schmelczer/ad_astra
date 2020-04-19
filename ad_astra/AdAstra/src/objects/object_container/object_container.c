#include "object_container.h"

#include "../../driver/redundant_storage/redundant_storage.h"
#include "../event_generator/event_generator.h"


Object* getFirstOfType(Prototype const* type) {
	for (uint8_t i = 0; i < OBJECT_COUNT; i++) {
		if (objects[i].prototype == type) {
			return objects + i;
		}
	}
	
	return NULL;
}

uint8_t getCountOf(Prototype const* type) {
	uint8_t count = 0;
	for (uint8_t i = 0; i < OBJECT_COUNT; i++) {
		if (objects[i].prototype == type) {
			count++;
		}
	}

	return count;
}

Object* getIntersectingObjectOfType(Rectangle boundingBox, Prototype const* type) {
	for (uint8_t i = 0; i < OBJECT_COUNT; i++) {
		if (objects[i].prototype == type && areIntersecting(boundingBox, getBoundingBox(objects + i))) {
			return objects + i;
		}
	}
	
	return NULL;
}

void clearObject(Object* object) {
	createObject(NULL, object);
}

void drawObjects(Rectangle window) {
	for (uint8_t i = 0; i < OBJECT_COUNT; i++) {
		drawObject(objects + i, window);
	}
}

void tickObjects(uint8_t previousFrameTime) {
	for (uint8_t i = 0; i < OBJECT_COUNT; i++) {
		tickObject(objects + i, previousFrameTime);
	}
}

void saveObject(Object* object) {
	DTO dto = {object->position, object->as};
	scheduleNextObjectForSave((uint8_t*)&dto, sizeof(dto));
}

void loadObject(Object* holder, Prototype const* prototype) {
	DTO dto;
	holder->prototype = loadNextObject((uint8_t*)&dto, sizeof(dto)) ? prototype : NULL;
	holder->position = dto.position;
	holder->as = dto.as;
}

void saveObjects() {
	saveObject(spaceshipObject);

	for (uint8_t i = 0; i < OBJECT_COUNT; i++) {
		if (objects[i].prototype == &Astronaut) {
			saveObject(objects + i);
		}
	}
}

void initializeObjectContainer() {
	for (uint8_t i = 0; i < OBJECT_COUNT; i++) {
		clearObject(objects + i);
	}
	
	createTransientObjects();
	
	if (isValid()) {
		loadObject(spaceshipObject, &Spaceship);
		loadObject(character, &Astronaut);
		loadObject(getEmptyObjectSpace(), &Astronaut);
		loadObject(getEmptyObjectSpace(), &Astronaut);
	} else {
		createPersistentObjects();
	}
}
