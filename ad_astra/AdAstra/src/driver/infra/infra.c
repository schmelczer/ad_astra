#include "infra.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "bitwise.h"
#include "../../hardware_access/hardware_access.h"


// (0.5625 + (0.5625 + 1.6875) / 2) / 1000 / timer interval
#define MEAN_OF_0_1_BIT_TIMES 53
// 9 / 2 / 1000 / timer interval
#define MAYBE_ONE_CHECK_TIME 141
// some large value
#define TIMEOUT 254

typedef enum {
	idle, maybeLeadingOne, leadingOneConfirmed, activeFirstBit, active, timingOut
} ProtocolState;

typedef enum {
	noMatch, foundStartingByte, significantByte, waitingForEndOfCommand, waitingForRepeat
} CommandState;

static struct {
	uint8_t current;
	uint8_t bitPosition;
	ProtocolState protocolState;
	CommandState commandState;
	OnCommandReceived onCommandReceived;
	OnReceiveStarted onReceiveStarted;
} infra;


static void saveCurrentByte() {
	uint8_t byte = infra.current;
	infra.current = 0;
	infra.bitPosition = 0;

	if (byte == INFRA_ADDRESS) {
		infra.commandState = foundStartingByte;
		return;
	}
	
	switch (infra.commandState) {
		case foundStartingByte:
			infra.commandState = significantByte;
			break;
		case significantByte:
			infra.onCommandReceived(byte);
			infra.commandState = waitingForEndOfCommand;
			break;
		case waitingForEndOfCommand:
			if (byte == 0) {
				infra.commandState = waitingForRepeat;
			}
			break;
		case waitingForRepeat:
			if (byte == 0) {
				infra.onCommandReceived(REPEAT_CODE);
			}
			break;
		default:
			break;
	}
}

static void saveBit(uint8_t bit) {
	infra.current <<= 1;
	infra.current |= bit;
	if (++infra.bitPosition == 8) {
		saveCurrentByte();
	}
}

static inline uint8_t isIrOff() {
	return getBit(PINB, IR_PIN);
}

static inline uint8_t isIrOn() {
	return !isIrOff();
}

ISR(PCINT0_vect) {
	infra.onReceiveStarted();
		
	switch (infra.protocolState) {
		case idle:
			if (isIrOn()) {
				enableTimerB(MAYBE_ONE_CHECK_TIME);
				infra.protocolState = maybeLeadingOne;
			}
			break;
		case maybeLeadingOne:
			infra.protocolState = idle;
			break;
		case leadingOneConfirmed:
			if (isIrOff()) {
				infra.protocolState = activeFirstBit;
			}
			break;
		case activeFirstBit:
		case timingOut:
			if (isIrOn()) {
				infra.protocolState = active;
				enableTimerB(MEAN_OF_0_1_BIT_TIMES);
			}
			break;
		case active:
			if (isIrOn()) {
				saveBit(1);
				enableTimerB(MEAN_OF_0_1_BIT_TIMES);
			}
			break;
	}
}

ISR(TIM0_COMPB_vect) {
	switch (infra.protocolState) {
		case maybeLeadingOne:
			if (isIrOn()) {
				infra.protocolState = leadingOneConfirmed;
			}
			disableTimerB();
			break;
		case active:
			saveBit(0);
			infra.protocolState = timingOut;
			enableTimerB(TIMEOUT);
			break;
		case timingOut:
			infra.protocolState = idle;
			saveCurrentByte();
			disableTimerB();
			break;
		default:
			disableTimerB();
			break;
	}
}

void initializeInfra(OnCommandReceived onCommandReceived, OnReceiveStarted onReceiveStarted) {
	setBit(PORTB, IR_PIN);	// enable pull-up
	setBit(PCMSK, IR_PIN);	// specific pin change interrupt enable
	setBit(GIMSK, PCIE);	// global on pin change interrupt enable;
	infra.onCommandReceived = onCommandReceived;
	infra.onReceiveStarted = onReceiveStarted;
}
