#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "../../prototype.h"


#define STAR_COUNT 8
#define STAR_SIZE 3
#define STAR_SHAPE_COUNT 3

const Prototype Background;
struct _background_t {
	uint8_t movementState;	
};
	
void initializeBackground();

#endif