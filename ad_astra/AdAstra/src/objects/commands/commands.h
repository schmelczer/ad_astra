#ifndef COMMANDS_H
#define COMMANDS_H

// There can be no more than COMMAND_BUFFER_SIZE commands
// waiting for processing simultaneously 
#define COMMAND_BUFFER_SIZE 8

// increaseContrast and decreaseContrast changes the contrast
// with this value
#define CONTRAST_STEP 15

// The possible inputs of the system
// Coincidentally these are the codes of the IR remote
// controller's buttons.
typedef enum {
	noCommand = 0,
	repeat = 1,
	increaseContrast = 87,
	decreaseContrast = 31,
	moveUp = 231,
	moveDown = 181,
	moveLeft = 239,
	moveRight = 165,
	action = 199,
} Command;

// Add a new command to the buffer
// It will not be processed immediately.
void addCommand(Command command);

// Process every command in the buffer at once in a FIFO manner
void handleCommands();
	
#endif
