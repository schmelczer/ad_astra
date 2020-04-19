#ifndef EVENT_GENERATOR_H
#define EVENT_GENERATOR_H

#define MAX_ASTEROID_COUNT 2

// For minimizing code size the position of generated objects is decided randomly. 
// If it fits then it stays. For each generated object can be a maximum of
// TRY_COUNT tries.
#define TRY_COUNT 16

// Create the background
void createTransientObjects();

// Create the spaceship and the player's astronaut
void createPersistentObjects();

// Generate asteroids and astronaut randomly based on a set of conditions
void generateEvents();

#endif
