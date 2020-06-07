#ifndef AI_H
#define AI_H

/// Between AI astronauts do actions
/// there has to be at least this many frames
#define AI_ACTION_INTERVAL 15

/// If there are non player controlled astronauts
/// control them according to some basic rule set
void handleAI();

#endif
