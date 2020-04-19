#ifndef BULLET_H
#define BULLET_H

#include "../../prototype.h"
#include <stdbool.h>


#define BULLET_SIZE ((Vec2){5, 1})


const Prototype Bullet;
struct _bullet_t {
	bool wereIntersectingInThePreviousFrame;
};

#endif
