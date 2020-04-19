#ifndef HEART_H
#define HEART_H

#include "../../prototype.h"


#define HEART_SIZE ((Vec2){7, 6})
#define TIME_TO_LIVE 128
#define HEART_FRAME_COUNT 2
#define HEART_ANIMATION_INTERVAL 12


const Prototype Heart;


struct _heart_t {
	uint8_t animationFrame;
	uint8_t timeLived;
	uint8_t timeSinceLastChange;
};

#endif
