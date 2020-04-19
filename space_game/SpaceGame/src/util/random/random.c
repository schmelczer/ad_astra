#include "random.h"


static uint16_t currentValue = SEED;

static uint16_t getRandom16bitNumberModuloPrime() {
	uint16_t bit = currentValue ^ (currentValue >> 2) ^ (currentValue >> 3) ^ (currentValue >> 5);
	currentValue = (currentValue >> 1) | (bit << 15);
	return currentValue % 1031;
}

uint8_t getRandomNumber() {
	return (uint8_t)(getRandom16bitNumberModuloPrime() ^ getRandom16bitNumberModuloPrime());
}
