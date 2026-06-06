#ifndef TEXT_H
#define TEXT_H

#include "../../prototype.h"
#include <stdbool.h>


#define TEXT_SIZE ((Vec2){64, 8})

const Prototype Text;
struct _text_t {
	int16_t xPosition;
	uint16_t textStartIndex;
	uint8_t timeSinceLastShift;
	bool hasSetCanCreate;
};

bool createText(Object* position);
void speedUpText();

#endif
