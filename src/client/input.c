#include "input.h"
#include "raylib.h"

void Cursor_Init(Cursor* cursor)
{
    // Start at bottom-left of board
    cursor->x = 0;
    cursor->y = BOARD_HEIGHT - 1;
}

void Cursor_Clamp(Cursor* cursor)
{
    if (cursor->x < 0) cursor->x = 0;
    if (cursor->x >= BOARD_WIDTH - 1) cursor->x = BOARD_WIDTH - 2;  // Cursor is 2 blocks wide
    if (cursor->y < 0) cursor->y = 0;
    if (cursor->y >= BOARD_HEIGHT) cursor->y = BOARD_HEIGHT - 1;
}

bool Cursor_HandleInput(Cursor* cursor)
{
    bool moved = false;

    if (IsKeyPressed(KEY_LEFT)) {
        cursor->x--;
        moved = true;
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        cursor->x++;
        moved = true;
    }
    if (IsKeyPressed(KEY_UP)) {
        cursor->y--;
        moved = true;
    }
    if (IsKeyPressed(KEY_DOWN)) {
        cursor->y++;
        moved = true;
    }

    if (moved) {
        Cursor_Clamp(cursor);
    }

    return moved;
}

bool Input_SwapPressed(void)
{
    return IsKeyPressed(KEY_SPACE);
}
