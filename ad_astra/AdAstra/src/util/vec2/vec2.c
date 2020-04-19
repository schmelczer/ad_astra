#include "vec2.h"


const Vec2 directions[] = {
	[north] = (Vec2){0, -1},
	[west] = (Vec2){-1, 0},
	[south] = (Vec2){0, 1},
	[east] = (Vec2){1, 0}
};

Vec2 add(Vec2 v1, Vec2 v2) {
	return (Vec2){v1.x + v2.x, v1.y + v2.y};
}

Vec2 substract(Vec2 v1, Vec2 v2) {
	return (Vec2){v1.x - v2.x, v1.y - v2.y};
}

Vec2 clampVec2(Vec2 v) {
	return (Vec2){
		v.x == 0 ? 0 : (v.x > 0 ? 1 : -1),
		v.y == 0 ? 0 : (v.y > 0 ? 1 : -1)
	};
}
