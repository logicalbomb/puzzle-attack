#ifndef MATCH_DETECTION_H
#define MATCH_DETECTION_H

#include "game_board.h"

// Minimum number of blocks required for a match
#define MIN_MATCH_LENGTH 3

// Detect all matches on the board
// Marks matched blocks with STATE_MATCHED
// Returns the number of blocks matched (0 if no matches)
int DetectMatches(GameBoard* board);

// Check if any blocks are currently marked as matched
bool HasMatchedBlocks(const GameBoard* board);

#endif // MATCH_DETECTION_H
