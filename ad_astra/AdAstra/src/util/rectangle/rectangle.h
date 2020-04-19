#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <stdbool.h>

#include "../vec2/vec2.h"


typedef struct {
	Vec2 position;
	Vec2 size;
} Rectangle;


bool areIntersecting(Rectangle r1, Rectangle r2);

// Return true only if inner is fully inside of outer
bool isInside(Rectangle inner, Rectangle outer);

// Return the geometrical middle point of the given rectangle
Vec2 getCenter(Rectangle r);

// Return a new rectangle shifted by vector translate
Rectangle translateRectangle(Rectangle r, Vec2 translate);

#endif
