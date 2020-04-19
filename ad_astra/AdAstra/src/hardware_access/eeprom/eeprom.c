#include "eeprom.h"

#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "bitwise.h"


uint8_t memory[STORAGE_SIZE] EEMEM;

static volatile struct {
	uint8_t* buffer;
	uint8_t positon;
	bool isWriting;
	OnEEPROMFinished onFinished;
} eeprom;

uint8_t loadByteEEPROM(uint8_t* address) {
	while(getBit(EECR, EEPE)) {}
	
	EEAR = (uint16_t)address;
	setBit(EECR, EERE);
	return EEDR;
}

uint8_t loadSavedByteEEPROM(uint8_t address) {
	return loadByteEEPROM(memory + address);
}

uint16_t loadWordEEPROM(uint16_t const* address) {
	uint16_t high = ((uint16_t)loadByteEEPROM((uint8_t*)address + 1)) << 8;
	uint16_t low = (uint16_t)loadByteEEPROM((uint8_t*)address);
	return high | low;
}

static inline void saveByteEEPROM(uint8_t* address, uint8_t byte) {
	if (byte != loadByteEEPROM(address)) {
		EEDR = byte;
		setBit(EECR, EEMPE);
		setBit(EECR, EEPE);
	}
}

void enableWritingEEPROM() {
	modifyBit(EECR, EERIE, eeprom.isWriting);
}

void disableWritingEEPROM() {
	clearBit(EECR, EERIE);		// clear eeprom ready interrupt enable
}

bool asyncWriteEEPROM(uint8_t* buffer, OnEEPROMFinished onFinished) {
	if (eeprom.isWriting) {
		return false;
	}

	eeprom.buffer = buffer;
	eeprom.onFinished = onFinished;
	eeprom.positon = 0;
	eeprom.isWriting = true;
	setBit(EECR, EERIE);	// set eeprom ready interrupt

	return true;
}

ISR(EE_RDY_vect) {	
	saveByteEEPROM(memory + eeprom.positon, eeprom.buffer[eeprom.positon]);
	
	if (++eeprom.positon == STORAGE_SIZE) {
		clearBit(EECR, EERIE);
		eeprom.isWriting = false;
		eeprom.onFinished(eeprom.buffer);
	}
}
