#include "physics.h"

static const float GRAVITY_DURATION = 0.15f;  // seconds per cell fallen

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
