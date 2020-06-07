#ifndef RANDOM_H
#define RANDOM_H

#include <avr/io.h>


/// Mustn't be zero, should be lower than 65535
#define SEED 42

/// Simple LFSR with some improvements to enhance distribution
/// while maintaining short execution time
uint8_t getRandomNumber();

#endif
