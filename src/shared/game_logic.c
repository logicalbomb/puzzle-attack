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

void SwapBlocks(GameBoard* board, int x, int y)
{
    if (!GameBoard_IsValidPosition(x, y) || !GameBoard_IsValidPosition(x + 1, y)) {
        return;
    }

    uint16_t left = GameBoard_GetCell(board, x, y);
    uint16_t right = GameBoard_GetCell(board, x + 1, y);

    GameBoard_SetCell(board, x, y, right);
    GameBoard_SetCell(board, x + 1, y, left);
}
