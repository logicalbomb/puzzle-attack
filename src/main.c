#include "raylib.h"
#include "game_board.h"
#include "game_logic.h"
#include "renderer.h"
#include "input.h"

int main(void)
{
    // Initialize game board with random blocks
    GameBoard board;
    GameBoard_Init(&board);
    GameBoard_FillRandom(&board);

    // Initialize cursor
    Cursor cursor;
    Cursor_Init(&cursor);

    // Initialize swap animation
    SwapAnimation swapAnim;
    SwapAnimation_Init(&swapAnim);

    // Initialize window
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Puzzle Attack");
    SetTargetFPS(60);

    // Calculate centered board position
    int boardX = Renderer_GetCenteredOffsetX();
    int boardY = Renderer_GetCenteredOffsetY();

    // Main game loop
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        // Handle cursor movement (always allowed)
        Cursor_HandleInput(&cursor);

        // Handle swap input (only when not animating)
        if (!swapAnim.active && Input_SwapPressed()) {
            SwapBlocks(&board, cursor.x, cursor.y);
            SwapAnimation_Start(&swapAnim, cursor.x, cursor.y);
        }

        // Update swap animation
        SwapAnimation_Update(&swapAnim, deltaTime);

        // Rendering
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw the game board with swap animation
        Renderer_DrawBoardWithSwap(&board, boardX, boardY, &swapAnim);

        // Draw cursor
        Renderer_DrawCursor(cursor.x, cursor.y, boardX, boardY);

        // Draw UI text
        DrawText("Puzzle Attack", 10, 10, 20, WHITE);
        DrawText("Arrow keys: move | SPACE: swap", 10, 35, 16, GRAY);
        DrawFPS(WINDOW_WIDTH - 80, 10);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
