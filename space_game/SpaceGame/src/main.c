#include "mediator/mediator.h"


// Stemming from the module oriented nature of the project
// there is a module responsible for setting up and orchestrating
// the other modules.
//
// From the main function we only have to instruct the afromentioned
// mediator module to do its job.

int main(void) {
	setupConnections();
	startGame();
}
