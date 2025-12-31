#include "physics.h"
#include "rng.h"

static const float GRAVITY_DURATION = 0.15f;  // seconds per cell fallen
static const float RISE_DURATION = 0.15f;     // seconds for rise animation

// Get a random block type using RNG
static BlockType GetRandomBlockType(RNG* rng)
{
    return (BlockType)(1 << RNG_Range(rng, BLOCK_TYPE_COUNT));
}

// Check if placing a block at (x, y) would create a horizontal match
static bool WouldMatchHorizontal(const GameBoard* board, int x, int y, BlockType type)
{
    if (x < 2) return false;

    BlockType left1 = BLOCK_TYPE(GameBoard_GetCell(board, x - 1, y));
    BlockType left2 = BLOCK_TYPE(GameBoard_GetCell(board, x - 2, y));

    return (left1 == type && left2 == type);
}

// Check if placing a block at (x, y) would create a vertical match
static bool WouldMatchVertical(const GameBoard* board, int x, int y, BlockType type)
{
    if (y < 2) return false;

    BlockType up1 = BLOCK_TYPE(GameBoard_GetCell(board, x, y - 1));
    BlockType up2 = BLOCK_TYPE(GameBoard_GetCell(board, x, y - 2));

    return (up1 == type && up2 == type);
}

void GravityAnimation_Init(GravityAnimation* anim)
{
    anim->active = false;
    anim->progress = 0.0f;
    anim->duration = GRAVITY_DURATION;
    anim->count = 0;
}

bool ApplyGravity(GameBoard* board, GravityAnimation* anim)
{
    anim->count = 0;
    int maxFallDistance = 0;

    // Process each column independently
    for (int x = 0; x < BOARD_WIDTH; x++) {
        // Track where the next block should land
        int writeY = BOARD_HEIGHT - 1;

        // Scan from bottom to top
        for (int readY = BOARD_HEIGHT - 1; readY >= 0; readY--) {
            uint16_t cell = GameBoard_GetCell(board, x, readY);
            BlockType type = BLOCK_TYPE(cell);

            if (type != BLOCK_EMPTY) {
                int fallDistance = writeY - readY;

                if (fallDistance > 0) {
                    // Move block down
                    GameBoard_SetCell(board, x, writeY, cell);
                    GameBoard_SetCell(board, x, readY, MAKE_BLOCK(BLOCK_EMPTY, STATE_NORMAL));

                    // Record for animation
                    if (anim->count < MAX_FALLING_BLOCKS) {
                        anim->blocks[anim->count].x = x;
                        anim->blocks[anim->count].y = writeY;
                        anim->blocks[anim->count].fallDistance = fallDistance;
                        anim->count++;
                    }

                    if (fallDistance > maxFallDistance) {
                        maxFallDistance = fallDistance;
                    }
                }

                writeY--;
            }
        }
    }

    // Start animation if any blocks moved
    if (anim->count > 0) {
        anim->active = true;
        anim->progress = 0.0f;
        // Scale duration based on max fall distance for consistent speed
        anim->duration = GRAVITY_DURATION * maxFallDistance;
        return true;
    }

    return false;
}

bool GravityAnimation_Update(GravityAnimation* anim, float deltaTime)
{
    if (!anim->active) {
        return false;
    }

    anim->progress += deltaTime / anim->duration;

    if (anim->progress >= 1.0f) {
        anim->progress = 1.0f;
        anim->active = false;
        return true;  // Animation completed
    }

    return false;
}

bool GravityAnimation_IsBlockFalling(const GravityAnimation* anim, int x, int y)
{
    if (!anim->active) {
        return false;
    }

    for (int i = 0; i < anim->count; i++) {
        if (anim->blocks[i].x == x && anim->blocks[i].y == y) {
            return true;
        }
    }
    return false;
}

bool GravityAnimation_AddBlock(GravityAnimation* anim, int x, int y, int fallDistance)
{
    if (!anim->active || anim->count >= MAX_FALLING_BLOCKS) {
        return false;
    }

    anim->blocks[anim->count].x = x;
    anim->blocks[anim->count].y = y;
    anim->blocks[anim->count].fallDistance = fallDistance;
    anim->count++;
    return true;
}

void RiseAnimation_Init(RiseAnimation* anim)
{
    anim->active = false;
    anim->progress = 0.0f;
    anim->duration = RISE_DURATION;
}

bool RaiseBoard(GameBoard* board, RiseAnimation* anim, RNG* rng)
{
    // Check if top row has any non-empty blocks (game over condition)
    for (int x = 0; x < BOARD_WIDTH; x++) {
        if (BLOCK_TYPE(GameBoard_GetCell(board, x, 0)) != BLOCK_EMPTY) {
            return false;  // Cannot raise - would lose blocks
        }
    }

    // Shift all blocks up by one row (from top to bottom)
    for (int y = 0; y < BOARD_HEIGHT - 1; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            uint16_t cell = GameBoard_GetCell(board, x, y + 1);
            GameBoard_SetCell(board, x, y, cell);
        }
    }

    // Generate new random blocks at the bottom row
    int bottomY = BOARD_HEIGHT - 1;
    for (int x = 0; x < BOARD_WIDTH; x++) {
        BlockType type;
        int retries = 0;
        const int maxRetries = 10;

        do {
            type = GetRandomBlockType(rng);
            retries++;
        } while ((WouldMatchHorizontal(board, x, bottomY, type) ||
                  WouldMatchVertical(board, x, bottomY, type)) &&
                 retries < maxRetries);

        GameBoard_SetCell(board, x, bottomY, MAKE_BLOCK(type, STATE_NORMAL));
    }

    // Start rise animation
    anim->active = true;
    anim->progress = 0.0f;

    return true;
}

bool RiseAnimation_Update(RiseAnimation* anim, float deltaTime)
{
    if (!anim->active) {
        return false;
    }

    anim->progress += deltaTime / anim->duration;

    if (anim->progress >= 1.0f) {
        anim->progress = 1.0f;
        anim->active = false;
        return true;  // Animation completed
    }

    return false;
}
