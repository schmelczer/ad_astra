#ifndef REDUNDANT_STORAGE_H
#define REDUNDANT_STORAGE_H

#include <stdbool.h>
#include <avr/io.h>


#define REDUNDANT_BUFFER_SIZE 24

void initializeRedundantStorage();

bool isValid();
void invalidateEEPROM();

bool startSchedulingObjectsForSaving();
void scheduleNextObjectForSave(uint8_t* object, uint8_t size);
void saveScheduledObjects();

// Returns is loaded object valid
bool loadNextObject(uint8_t* holder, uint8_t size);

#endif
