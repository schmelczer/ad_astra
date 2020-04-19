#include "redundant_storage.h"

#include "../../hardware_access/hardware_access.h"

#define offsetof(type, member)  __builtin_offsetof (type, member)

#define OBJECT_VALIDITY_MASK (0b00111111)
#define IS_USING_PAGE_A_BIT 6
#define IS_VALID_BIT 7
#define METADATA_REDUNDANCY 4

typedef struct {
	uint8_t redundantBufferA[REDUNDANT_BUFFER_SIZE];
	uint8_t redundantBufferB[REDUNDANT_BUFFER_SIZE];
	uint8_t metadata[METADATA_REDUNDANCY];
} MemoryLayout;

static volatile struct {
	MemoryLayout buffer;
	uint8_t saveIndex;
	uint8_t* savePointer;
	uint8_t loadIndex;
	uint8_t* loadPointer;
	uint8_t validMetadata;
	bool isEEPROMWriting;
} storage;

static inline bool isUsingPageA() {
	return getBit(storage.validMetadata, IS_USING_PAGE_A_BIT);
}

bool isValid() {
	return getBit(storage.validMetadata, IS_VALID_BIT);
}

static inline void fixMetadata() {
	uint8_t a = storage.buffer.metadata[0];
	uint8_t b = storage.buffer.metadata[1];
	uint8_t c = storage.buffer.metadata[2];
	uint8_t d = storage.buffer.metadata[3];
	
	if (a == b || a == c || a == d) {
		storage.validMetadata = a;
	} else if (b == c || b == d) {
		storage.validMetadata = b;
	} else {
		storage.validMetadata = c;
	}
}

void initializeRedundantStorage() {
	for (uint8_t i = 0; i < sizeof(MemoryLayout); i++) {
		((uint8_t*)&storage.buffer)[i] = loadSavedByteEEPROM(i);
	}
	
	fixMetadata();
}

void invalidateEEPROM() {
	for (uint8_t i = 0; i < sizeof(MemoryLayout); i++) {
		((uint8_t*)&storage.buffer)[i] = 0;
	}
	
	storage.validMetadata = 0;
}

void onEEPROMWriteFinished(__attribute__((unused)) uint8_t* _) {
	storage.isEEPROMWriting = false;
}

bool startSchedulingObjectsForSaving() {
	if (storage.isEEPROMWriting) {
		return false;
	}
	
	toggleBit(storage.validMetadata, IS_USING_PAGE_A_BIT);
	storage.validMetadata &= ~OBJECT_VALIDITY_MASK;
	storage.saveIndex = 0;
	storage.savePointer = isUsingPageA() ? 
		  storage.buffer.redundantBufferA
		: storage.buffer.redundantBufferB;
	
	return true;
}

void scheduleNextObjectForSave(uint8_t* object, uint8_t size) {
	setBit(storage.validMetadata, storage.saveIndex);
	storage.saveIndex++;
	for (uint8_t i = 0; i < size; i++) {
		*storage.savePointer = object[i];
		storage.savePointer++;
	}
}

void saveScheduledObjects() {
	setBit(storage.validMetadata, IS_VALID_BIT);
	
	for (uint8_t i = 0; i < METADATA_REDUNDANCY; i++) {
		storage.buffer.metadata[i] = storage.validMetadata;
	}
	
	storage.isEEPROMWriting = true;
	asyncWriteEEPROM((uint8_t*)&storage.buffer, onEEPROMWriteFinished);
}

bool loadNextObject(uint8_t* holder, uint8_t size) {
	if (storage.loadIndex++ == 0) {
		storage.loadPointer = isUsingPageA() ?
			  storage.buffer.redundantBufferA
			: storage.buffer.redundantBufferB;
	}
	
	if (getBit(storage.validMetadata, storage.loadIndex - 1)) {
		for (uint8_t i = 0; i < size; i++) {
			holder[i] = *storage.loadPointer++;
		}
		return true;
	}
	return false;
}
