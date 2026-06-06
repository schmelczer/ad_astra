#include "mediator.h"

#include <stdbool.h>

#include "../hardware_access/hardware_access.h"
#include "../objects/object_container/object_container.h"
#include "../objects/event_generator/event_generator.h"
#include "../objects/commands/commands.h"
#include "../objects/ai/ai.h"
#include "../driver/display/display.h"
#include "../driver/infra/infra.h"
#include "../driver/sleep/sleep.h"

#define TARGET_FRAME_DURATION 20	// ms
#define DEATH_SCREEN_LENGTH 50		// frames
#define SAVE_INTERVAL 50			// every x frames

static struct {
	uint8_t contrast;
	uint8_t framesSinceLastSave;
	uint8_t deathDownCounter;
	uint8_t receivedWakeUpBitCount;
} state = {
	.contrast = 255
};

static inline bool handleDeathAnimation() {
	if (isSpaceshipDestroyed()) {
		setDisplayContrast(state.contrast * (state.deathDownCounter / DEATH_SCREEN_LENGTH));
		if (state.deathDownCounter-- == 0) {
			return false;
		};
	}
	return true;
}

static bool frameFunction(uint8_t previousFrameTime) {
	tickObjects(previousFrameTime);
	handleCommands();
	handleAI();
	generateEvents();
	drawFrame();
	return handleDeathAnimation();
}

static inline void initializeGame() {
	state.deathDownCounter = DEATH_SCREEN_LENGTH;
	setDisplayContrast(state.contrast);
	initializeBackground();
	initializeObjectContainer();
}

void setupConnections() {
	initializeHardwareAccess();
	initializeInfra(addCommand);
	initializeDisplay(drawObjects);
}

void startGame() {
	while (true) {
		initializeGame();
		startFrameLoop(frameFunction, TARGET_FRAME_DURATION);
	}
}

void changeDisplayContrast(int8_t by) {
	if (by < 0) {
		state.contrast = (state.contrast < -by) ? 0 : (state.contrast + by);
	} else {
		state.contrast = (state.contrast > 255 - by) ? 255 : (state.contrast + by);
	}
	
	setDisplayContrast(state.contrast);
}
