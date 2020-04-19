#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <avr/io.h>


void setupConnections();
void startGame();
void handleOff();
void changeDisplayContrast(int8_t by);

#endif
