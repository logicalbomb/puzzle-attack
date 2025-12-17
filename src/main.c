#include "raylib.h"

int main(void)
{
    // Initialize window
    InitWindow(800, 600, "Puzzle Attack");
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Puzzle Attack", 280, 260, 40, DARKGRAY);
        DrawText("Press ESC to quit", 300, 320, 20, GRAY);
        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
