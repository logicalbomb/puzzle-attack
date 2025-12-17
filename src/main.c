#include "raylib.h"
#include "game_board.h"
#include "renderer.h"

int main(void)
{
    // Initialize game board
    GameBoard board;
    GameBoard_Init(&board);

    // Add some test blocks to visualize
    GameBoard_SetCell(&board, 0, 11, MAKE_BLOCK(BLOCK_RED, STATE_NORMAL));
    GameBoard_SetCell(&board, 1, 11, MAKE_BLOCK(BLOCK_BLUE, STATE_NORMAL));
    GameBoard_SetCell(&board, 2, 11, MAKE_BLOCK(BLOCK_GREEN, STATE_NORMAL));
    GameBoard_SetCell(&board, 3, 11, MAKE_BLOCK(BLOCK_YELLOW, STATE_NORMAL));
    GameBoard_SetCell(&board, 4, 11, MAKE_BLOCK(BLOCK_PURPLE, STATE_NORMAL));
    GameBoard_SetCell(&board, 5, 11, MAKE_BLOCK(BLOCK_RED, STATE_NORMAL));
    GameBoard_SetCell(&board, 0, 10, MAKE_BLOCK(BLOCK_BLUE, STATE_NORMAL));
    GameBoard_SetCell(&board, 1, 10, MAKE_BLOCK(BLOCK_GREEN, STATE_NORMAL));
    GameBoard_SetCell(&board, 2, 10, MAKE_BLOCK(BLOCK_YELLOW, STATE_NORMAL));

    // Initialize window
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Puzzle Attack");
    SetTargetFPS(60);

    // Calculate centered board position
    int boardX = Renderer_GetCenteredOffsetX();
    int boardY = Renderer_GetCenteredOffsetY();

    // Main game loop
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw the game board
        Renderer_DrawBoard(&board, boardX, boardY);

        // Draw UI text
        DrawText("Puzzle Attack", 10, 10, 20, WHITE);
        DrawText("Press ESC to quit", 10, 35, 16, GRAY);
        DrawFPS(WINDOW_WIDTH - 80, 10);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
