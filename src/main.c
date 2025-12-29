#include "raylib.h"
#include "game_board.h"
#include "game_logic.h"
#include "match_detection.h"
#include "physics.h"
#include "renderer.h"
#include "input.h"
#include "ui.h"

// Clear animation timing
static const float CLEAR_DELAY = 0.3f;  // Time to show matched blocks before clearing

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

    // Initialize gravity animation
    GravityAnimation gravityAnim;
    GravityAnimation_Init(&gravityAnim);

    // Initialize rise animation
    RiseAnimation riseAnim;
    RiseAnimation_Init(&riseAnim);

    // Track match/clear state
    int lastMatchCount = 0;
    float clearTimer = 0.0f;
    bool waitingToClear = false;

    // Combo tracking
    int comboCount = 0;

    // Initialize UI state
    UIState ui;
    UI_Init(&ui);

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

        // Handle swap input
        // Allowed during fall/clear animations, blocked during swap/rise animations
        // Cannot swap blocks that are currently falling
        bool canSwap = !swapAnim.active && !riseAnim.active;
        if (canSwap && Input_SwapPressed()) {
            // Check if either block under cursor is falling
            bool leftFalling = GravityAnimation_IsBlockFalling(&gravityAnim, cursor.x, cursor.y);
            bool rightFalling = GravityAnimation_IsBlockFalling(&gravityAnim, cursor.x + 1, cursor.y);

            if (!leftFalling && !rightFalling) {
                if (SwapBlocks(&board, cursor.x, cursor.y)) {
                    SwapAnimation_Start(&swapAnim, cursor.x, cursor.y);
                }
            }
        }

        // Handle raise input (only when not animating)
        bool animating = swapAnim.active || gravityAnim.active || riseAnim.active || waitingToClear;
        if (!animating && Input_RaisePressed()) {
            RaiseBoard(&board, &riseAnim);
        }

        // Update swap animation
        bool swapCompleted = SwapAnimation_Update(&swapAnim, deltaTime);

        // Update gravity animation
        bool gravityCompleted = GravityAnimation_Update(&gravityAnim, deltaTime);

        // Update rise animation
        bool riseCompleted = RiseAnimation_Update(&riseAnim, deltaTime);

        // Check for matches after swap completes
        if (swapCompleted) {
            lastMatchCount = DetectMatches(&board);
            if (lastMatchCount > 0) {
                comboCount = 1;  // Start new combo chain
                ui.displayCombo = 1;
                ui.lastClearCount = lastMatchCount;
                ui.showingMatch = true;
                waitingToClear = true;
                clearTimer = CLEAR_DELAY;
            } else {
                // No matches - apply gravity (handles swapping into empty space)
                ApplyGravity(&board, &gravityAnim);
            }
        }

        // Check for matches after gravity completes (cascade)
        if (gravityCompleted) {
            lastMatchCount = DetectMatches(&board);
            if (lastMatchCount > 0) {
                comboCount++;  // Increment combo for cascade
                ui.displayCombo = comboCount;
                ui.lastClearCount = lastMatchCount;
                ui.showingMatch = true;
                waitingToClear = true;
                clearTimer = CLEAR_DELAY;
            } else {
                // Cascade ended, reset combo
                comboCount = 0;
            }
        }

        // Check for matches after rise completes
        if (riseCompleted) {
            lastMatchCount = DetectMatches(&board);
            if (lastMatchCount > 0) {
                comboCount = 1;  // Start new combo chain
                ui.displayCombo = 1;
                ui.lastClearCount = lastMatchCount;
                ui.showingMatch = true;
                waitingToClear = true;
                clearTimer = CLEAR_DELAY;
            }
        }

        // Update clear timer and clear matches when ready
        if (waitingToClear) {
            clearTimer -= deltaTime;
            if (clearTimer <= 0.0f) {
                ui.lastClearCount = ClearMatches(&board, comboCount);
                ui.showingMatch = false;
                waitingToClear = false;

                // Update high score
                UI_UpdateHighScore(&ui, board.score);

                // Apply gravity after clearing
                ApplyGravity(&board, &gravityAnim);
            }
        }

        // Rendering
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw the game board with all animations
        Renderer_DrawBoardWithAnimations(&board, boardX, boardY, &swapAnim, &gravityAnim, &riseAnim);

        // Draw cursor
        Renderer_DrawCursor(cursor.x, cursor.y, boardX, boardY);

        // Draw UI
        UI_Draw(&ui, &board, WINDOW_WIDTH);

        DrawFPS(WINDOW_WIDTH - 80, 30);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
