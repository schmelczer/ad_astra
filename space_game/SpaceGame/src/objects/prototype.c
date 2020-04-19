#include "prototype.h"


Prototype temp;

static void loadPrototype(Prototype* prototype) {
	/*for (uint8_t i = 0; i < sizeof(prototype); i++) {
		((uint8_t*)(&temp))[i] = pgm_read_byte(prototype + i);
	}*/
	
	temp = *prototype;
}

void tickObjectFromPrototype(Object* object) {
	loadPrototype(object->prototype);
	temp.tick(object);
}

void drawObjectFromPrototype(Object* object) {
	/*loadPrototype(object->prototype);
	temp.draw(object);*/
}
