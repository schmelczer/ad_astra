#ifndef OBJECT_HANDLER_H
#define OBJECT_HANDLER_H

#include <stdbool.h>
#include <avr/io.h>

#include "null.h"

#include "../object.h"
#include "../../util/rectangle/rectangle.h"
#include "../../util/vec2/vec2.h"


// Contain up to OBJECT_COUNT objects.
// Provide the basic functionality to access, search and modify
// these objects.
// For ease of use, there are some convenience global variables for accessing
// objects that are very commonly accessed.

#define OBJECT_COUNT 10
#define BACKGROUND_INDEX 0
#define SPACESHIP_INDEX 1
#define CHARACTER_INDEX 2

#define spaceshipObject (objects + SPACESHIP_INDEX)
#define character (objects + CHARACTER_INDEX)

// The actual container
Object objects[OBJECT_COUNT];

// may return NULL
Object* getFirstOfType(Prototype const* type);
// may return NULL
#define getEmptyObjectSpace() getFirstOfType(NULL)

// Return the number of objects with a prototype being type
uint8_t getCountOf(Prototype const* type);

// Return a reference to a random object intersecting boundingBox and having 
// a prototype of type
Object* getIntersectingObjectOfType(Rectangle boundingBox, Prototype const* type);

// Call the tick method of every object
// objects might respond to the elapsed time
void tickObjects(uint8_t previousFrameTime);

// Call the draw method of every object
void drawObjects(Rectangle window);

// Schedule the saving of persistent objects into redundant_storage
// Asteroids are not persistent.
void saveObjects();

// Delete the object given by its address from objects
// It achieves this by setting the object's prototype to NULL.
void clearObject(Object* object);

// Delete every object inside of objects 
// and create (or load) the starting objects
void initializeObjectContainer();

#endif
