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
#include "../driver/uart/transmit.h"
#include "../driver/uart/receive.h"


#define TARGET_FRAME_DURATION 20	/// ms
#define DEATH_SCREEN_LENGTH 50		/// frames
#define REPORT_INTERVAL 50			/// every x frames

static struct {
	uint8_t contrast;
	uint8_t framesSinceLastReport;
	uint8_t longestFrameTimeSinceLastReport;
	uint8_t deathDownCounter;
} state = {
	.contrast = 255
};

static bool frameFunction(uint8_t previousFrameTime) {
	if (previousFrameTime > state.longestFrameTimeSinceLastReport) {
		state.longestFrameTimeSinceLastReport = previousFrameTime;
	}
	
	if (++state.framesSinceLastReport >= REPORT_INTERVAL) {
		state.framesSinceLastReport = 0;
		sendUintOnUartAsync(state.longestFrameTimeSinceLastReport);
		sendTextOnUartAsync(" ms\r\n");
		state.longestFrameTimeSinceLastReport = 0;
	}
	
	if (spaceshipObject->as.spaceship.healthLoss >= MAX_HEALTH) {
		setDisplayContrast(state.contrast * (state.deathDownCounter / DEATH_SCREEN_LENGTH));
		if (state.deathDownCounter-- == 0) {
			return false;
		};
	}
	
	tickObjects(previousFrameTime);
	handleCommands();
	handleAI();
	generateEvents();
	drawFrame();
	
	return true;
}

static inline void startGameLogic() {
	state.deathDownCounter = DEATH_SCREEN_LENGTH;
	
	setDisplayContrast(255);
	initializeBackground();
	initializeObjectContainer();
	createObjects();
}

void addKeyboardCommand(uint8_t key) {
	switch (key) {
		case 'w':
			addCommand(moveUp);
			break;
		case 'a':
			addCommand(moveLeft);
			break;
		case 's':
			addCommand(moveDown);
			break;
		case 'd':
			addCommand(moveRight);
			break;
		case ' ':
			addCommand(action);
			break;
		case '+':
			addCommand(increaseContrast);
			break;
		case '-':
			addCommand(decreaseContrast);
			break;
	}
}

void setupConnections() {
	initializeHardwareAccess();
	initializeInfra(addCommand);
	initializeDisplay(drawObjects);
	initializeUartTransmit();
	initializeUartReceive(addKeyboardCommand);
}

void startGame() {
	while (true) {
		startGameLogic();
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
