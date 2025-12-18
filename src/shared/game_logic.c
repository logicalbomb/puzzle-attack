#include "game_logic.h"

static const float SWAP_DURATION = 0.15f;  // seconds

void SwapAnimation_Init(SwapAnimation* anim)
{
    anim->active = false;
    anim->x = 0;
    anim->y = 0;
    anim->progress = 0.0f;
    anim->duration = SWAP_DURATION;
}

void SwapAnimation_Start(SwapAnimation* anim, int x, int y)
{
    anim->active = true;
    anim->x = x;
    anim->y = y;
    anim->progress = 0.0f;
}

bool SwapAnimation_Update(SwapAnimation* anim, float deltaTime)
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

bool SwapBlocks(GameBoard* board, int x, int y)
{
    if (!GameBoard_IsValidPosition(x, y) || !GameBoard_IsValidPosition(x + 1, y)) {
        return false;
    }

    uint16_t left = GameBoard_GetCell(board, x, y);
    uint16_t right = GameBoard_GetCell(board, x + 1, y);

    // Cannot swap blocks that are in MATCHED state
    if (BLOCK_STATE(left) == STATE_MATCHED || BLOCK_STATE(right) == STATE_MATCHED) {
        return false;
    }

    GameBoard_SetCell(board, x, y, right);
    GameBoard_SetCell(board, x + 1, y, left);
    return true;
}

int ClearMatches(GameBoard* board)
{
    int clearedCount = 0;

    for (int i = 0; i < BOARD_SIZE; i++) {
        if (BLOCK_STATE(board->grid[i]) == STATE_MATCHED) {
            board->grid[i] = MAKE_BLOCK(BLOCK_EMPTY, STATE_NORMAL);
            clearedCount++;
        }
    }

    // Add score for cleared blocks
    if (clearedCount > 0) {
        board->score += clearedCount * SCORE_PER_BLOCK;

        // Apply bonus for larger clears
        if (clearedCount >= 5) {
            board->score += SCORE_BONUS_5_PLUS_MATCH;
        } else if (clearedCount >= 4) {
            board->score += SCORE_BONUS_4_MATCH;
        }
    }

    return clearedCount;
}
