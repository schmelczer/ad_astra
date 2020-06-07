#ifndef OBJECT_H
#define OBJECT_H

#include <avr/io.h>
#include <avr/pgmspace.h>

#include "../util/rectangle/rectangle.h"

#include "types/asteroid/asteroid.h"
#include "types/astronaut/astronaut.h"
#include "types/background/background.h"
#include "types/spaceship/spaceship.h"
#include "types/bullet/bullet.h"
#include "types/heart/heart.h"

#include "prototype.h"


/// Objects (they could have been called GameObjects) have a simple 
/// hierarchy. A prototype/flyweight motivated system is used.
/// Each type has some common data and methods which are stored
/// in their respective prototype. 
/// This module provides us with the methods to easily and mostly
/// transparently access an object's prototype.


typedef union {
	struct _background_t background;
	struct _spaceship_t spaceship;
	struct _astronaut_t astronaut;
	struct _asteroid_t asteroid;
	struct _bullet_t bullet;
	struct _heart_t heart;
} object_specific_data_t;

struct _object_t {
	Prototype const* prototype;
	Vec2 position;
	object_specific_data_t as;
};

/// Set the prototype of the holder and initialize all the holder's
/// vale to zero. Return the freshly updated holder
Object* createObject(Prototype const* prototype, Object* holder);

/// Call the tick function referenced in the object's prototype
/// on the object itself
/// Object might react to the elapsed time.
/// Does nothing when called with NULL.
void tickObject(Object* object, uint8_t previousFrameTime);

/// Call the draw function referenced in the object's prototype
/// on the object itself.
/// Does nothing when called with NULL.
void drawObject(Object* object, Rectangle compositingWindow);

/// Find out the prototype of the object and return the size of that
Vec2 getSize(Object const* object);
Vec2 getSizeFromPrototype(Prototype const* prototype);

/// Move the position of the object by a vector
void move(Object* object, Vec2 value);

/// Get a new rectangle from the objects position and its 
/// prototype's size
Rectangle getBoundingBox(Object const* object);

#endif
