#ifndef PHYSICS_H
#define PHYSICS_H

#include "game_board.h"
#include <stdbool.h>

// Maximum blocks that can fall simultaneously
#define MAX_FALLING_BLOCKS BOARD_SIZE

// Track a single falling block
typedef struct {
    int x, y;           // Current grid position (destination)
    int fallDistance;   // How many cells this block fell
} FallingBlock;

// Gravity animation state
typedef struct {
    bool active;
    float progress;     // 0.0 to 1.0
    float duration;     // Animation duration in seconds
    int count;          // Number of falling blocks
    FallingBlock blocks[MAX_FALLING_BLOCKS];
} GravityAnimation;

// Initialize gravity animation state
void GravityAnimation_Init(GravityAnimation* anim);

// Apply gravity to the board
// Moves blocks down to fill empty spaces
// Populates the animation with falling block info
// Returns true if any blocks moved
bool ApplyGravity(GameBoard* board, GravityAnimation* anim);

// Update gravity animation (call each frame with delta time)
// Returns true when animation completes
bool GravityAnimation_Update(GravityAnimation* anim, float deltaTime);

#endif // PHYSICS_H
