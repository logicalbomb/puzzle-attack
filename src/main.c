#include "raylib.h"
#include "game_board.h"
#include "game_logic.h"
#include "match_detection.h"
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

    // Track last match count for display
    int lastMatchCount = 0;

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
        bool swapCompleted = SwapAnimation_Update(&swapAnim, deltaTime);

        // Check for matches after swap completes
        if (swapCompleted) {
            lastMatchCount = DetectMatches(&board);
        }

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
        if (lastMatchCount > 0) {
            DrawText(TextFormat("Matched: %d blocks", lastMatchCount), 10, 55, 16, GREEN);
        }
        DrawFPS(WINDOW_WIDTH - 80, 10);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
