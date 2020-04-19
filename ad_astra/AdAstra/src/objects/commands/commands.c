#include "commands.h"

#include "../../objects/object_container/object_container.h"
#include "../../objects/types/spaceship/spaceship.h"
#include "../../mediator/mediator.h"


static struct {
	Command received[COMMAND_BUFFER_SIZE];
	uint8_t start;
	uint8_t end;
	Command previous;
} commands;


static inline bool areThereAnyCommandsLeft() {
	return commands.start != commands.end;
}

static inline Command getNextCommand() {
	Command top = commands.received[commands.start++];
	commands.start %= COMMAND_BUFFER_SIZE;
	return top;
}

void addCommand(Command command) {
	commands.received[commands.end++] = command;
	commands.end %= COMMAND_BUFFER_SIZE;
}

void handleCommands() {
	while(areThereAnyCommandsLeft()) {
		Command next = getNextCommand();
		if (next == repeat) {
			next = commands.previous;
		} else {
			commands.previous = next;
		}
		
		switch(next) {
			case increaseContrast:
				changeDisplayContrast(CONTRAST_STEP);
				break;
			case decreaseContrast:
				changeDisplayContrast(-CONTRAST_STEP);
				break;
			case moveLeft:
				moveAstronaut(character, directions[west]);
				break;
			case moveRight:
				moveAstronaut(character, directions[east]);
				break;
			case moveUp:
				moveAstronaut(character, directions[north]);
				break;
			case moveDown:
				moveAstronaut(character, directions[south]);
				break;
			case reset:
				destroySpaceship();
				commands.previous = noAction;
				break;
			case turnOff:
				handleOff();
				commands.previous = noAction;
				break;
			case action:
				makeAstronautDoAction(character);
				commands.previous = noAction;
				break;
			default:
				break;
		}
	}
}