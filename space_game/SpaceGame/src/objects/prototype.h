#ifndef PROTOTYPE_H
#define PROTOTYPE_H

#include <avr/io.h>
#include "../util/rectangle/rectangle.h"


/// See more information in object.h

struct _object_t;
typedef struct _object_t Object;

/// Update the inner state of the given object
/// The first argument is the object itself, the second is the elapsed
/// time in milliseconds.
typedef void (*TickMethod)(Object*, uint8_t);

/// Draw the given object if its overlapping with the given rectangle
typedef void (*DrawMethod)(Object*, Rectangle);

typedef struct {
	TickMethod tick;
	DrawMethod draw;
	Vec2 size;
} Prototype;

#endif
