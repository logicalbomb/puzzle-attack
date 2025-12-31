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

// Check if a block at (x, y) is currently falling
bool GravityAnimation_IsBlockFalling(const GravityAnimation* anim, int x, int y);

// Add a new falling block to an active animation
// Returns true if added, false if animation not active or array full
bool GravityAnimation_AddBlock(GravityAnimation* anim, int x, int y, int fallDistance);

// Rise animation state
typedef struct {
    bool active;
    float progress;     // 0.0 to 1.0
    float duration;     // Animation duration in seconds
} RiseAnimation;

// Initialize rise animation state
void RiseAnimation_Init(RiseAnimation* anim);

// Raise the board by one row
// Shifts all blocks up, adds new random row at bottom
// Populates the animation for visual effect
// Returns true if successful, false if top row has blocks (game over)
bool RaiseBoard(GameBoard* board, RiseAnimation* anim, RNG* rng);

// Update rise animation (call each frame with delta time)
// Returns true when animation completes
bool RiseAnimation_Update(RiseAnimation* anim, float deltaTime);

#endif // PHYSICS_H
