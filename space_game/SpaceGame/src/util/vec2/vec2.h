#ifndef VEC2_H
#define VEC2_H

#include <avr/io.h>


typedef struct {
	int8_t x;
	int8_t y;	
} Vec2;

typedef enum {
	north, west, south, east
} Direction;

/// The array directions can be indexed by a Direction and
/// it contains vectors pointing into that direction.
const Vec2 directions[4];

Vec2 add(Vec2 v1, Vec2 v2);
Vec2 substract(Vec2 v1, Vec2 v2);

/// Return new vector with ll components between -1 and 1 (inclusive)
Vec2 clampVec2(Vec2 vector);

#endif
