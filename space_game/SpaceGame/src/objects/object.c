#include "object.h"
#include "null.h"


Object* createObject(Prototype const* prototype, Object* holder) {
	Object empty = {0};
	*holder = empty;
	holder->prototype = prototype;
	return holder;
}

void tickObject(Object* object, uint8_t previousFrameTime) {
	if (object->prototype != NULL) {
		((TickMethod)pgm_read_word(&object->prototype->tick))(object, previousFrameTime);
	}
}

void drawObject(Object* object, Rectangle compositingWindow) {
	if (object->prototype != NULL && areIntersecting(getBoundingBox(object), compositingWindow)) {
		((DrawMethod)pgm_read_word(&object->prototype->draw))(object, compositingWindow);
	}
}

Vec2 getSizeFromPrototype(Prototype const* prototype) {
	// required for casting
	uint16_t read = pgm_read_word(&prototype->size);
	Vec2* v = (Vec2*) &read;
	return *v;
}

Vec2 getSize(Object const* object) {
	return getSizeFromPrototype(object->prototype);
}

void move(Object* object, Vec2 value) {
	object->position = add(object->position, value);
}

Rectangle getBoundingBox(Object const* object) {
	return (Rectangle){object->position, getSize(object)};
}
