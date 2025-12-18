#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "game_board.h"
#include <stdbool.h>

// Swap animation state
typedef struct {
    bool active;
    int x, y;           // Grid position (left block of pair)
    float progress;     // 0.0 to 1.0
    float duration;     // Animation duration in seconds
} SwapAnimation;

// Initialize swap animation state
void SwapAnimation_Init(SwapAnimation* anim);

// Start a swap animation
void SwapAnimation_Start(SwapAnimation* anim, int x, int y);

// Update swap animation (call each frame with delta time)
// Returns true when animation completes
bool SwapAnimation_Update(SwapAnimation* anim, float deltaTime);

// Swap the two blocks at cursor position
// Swaps blocks at (x, y) and (x+1, y)
// Returns true if swap succeeded, false if blocked (e.g., matched blocks)
bool SwapBlocks(GameBoard* board, int x, int y);

// Scoring constants
#define SCORE_PER_BLOCK 10
#define SCORE_BONUS_4_MATCH 20
#define SCORE_BONUS_5_PLUS_MATCH 50

// Clear all blocks marked as MATCHED
// Sets them to BLOCK_EMPTY and adds to score
// Returns the number of blocks cleared
int ClearMatches(GameBoard* board);

#endif // GAME_LOGIC_H
