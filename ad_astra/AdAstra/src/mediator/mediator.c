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
#include "../driver/redundant_storage/redundant_storage.h"


#define TARGET_FRAME_DURATION 20	// ms
#define DEATH_SCREEN_LENGTH 50		// frames
#define SAVE_INTERVAL 50			// every x frames

static struct {
	uint8_t contrast;
	uint8_t framesSinceLastSave;
	uint8_t deathDownCounter;
	bool isSleeping;
	uint8_t receivedWakeUpBitCount;
} state = {
	.contrast = 255
};

static inline void saveGame() {
	if (++state.framesSinceLastSave > SAVE_INTERVAL && startSchedulingObjectsForSaving()) {
		scheduleNextObjectForSave(&state.contrast, sizeof(state.contrast));
		saveObjects();
		saveScheduledObjects();
		state.framesSinceLastSave = 0;
	}
}

static inline bool handleDeathAnimation() {
	if (isSpaceshipDestroyed()) {
		setDisplayContrast(state.contrast * (state.deathDownCounter / DEATH_SCREEN_LENGTH));
		if (state.deathDownCounter-- == 0) {
			invalidateEEPROM();
			return false;
		};
	}
	return true;
}

void handleOff() {
	turnDisplayOnOff(false);
	state.isSleeping = true;
}

void handleOn() {
	if (state.isSleeping && ++state.receivedWakeUpBitCount > 50) {
		turnDisplayOnOff(true);
		state.isSleeping = false;
		state.receivedWakeUpBitCount = 0;
	}
}


static bool frameFunction(uint8_t previousFrameTime) {
	/*if (state.isSleeping) {
		powerOff();
	} else {*/
		disableWritingEEPROM();
		tickObjects(previousFrameTime);
		handleCommands();
		handleAI();
		generateEvents();
		drawFrame();
		enableWritingEEPROM();
		saveGame();
		return handleDeathAnimation();
	//}
	
	return true;
}

static inline void createObjects() {
	state.deathDownCounter = DEATH_SCREEN_LENGTH;
	
	if (isValid()) {
		loadNextObject(&state.contrast, sizeof(state.contrast));
	}
	setDisplayContrast(state.contrast);
	
	initializeBackground();
	initializeObjectContainer();
}

void setupConnections() {
	initializeHardwareAccess();
	initializeRedundantStorage();
	initializeInfra(addCommand, handleOn);
	initializeDisplay(drawObjects);
}

void startGame() {
	while (true) {
		createObjects();
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
