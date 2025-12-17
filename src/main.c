#include "raylib.h"
#include "game_board.h"
#include "renderer.h"

int main(void)
{
    // Initialize game board with random blocks
    GameBoard board;
    GameBoard_Init(&board);
    GameBoard_FillRandom(&board);

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
