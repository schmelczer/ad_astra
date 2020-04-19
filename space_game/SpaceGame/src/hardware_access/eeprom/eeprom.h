#ifndef EEPROM_H
#define EEPROM_H

#include <avr/io.h>


#define STORAGE_SIZE 52

typedef void (*OnEEPROMFinished)(uint8_t*);

inline void initializeEEPROM() {
	EECR = 0;		// atomic write
}

#endif
