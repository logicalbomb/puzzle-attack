#include "raylib.h"
#include "game_board.h"
#include "game_logic.h"
#include "match_detection.h"
#include "physics.h"
#include "renderer.h"
#include "input.h"
#include "ui.h"
#include "game_state.h"

// Clear animation timing
static const float CLEAR_DELAY = 0.3f;  // Time to show matched blocks before clearing

// Helper to reset game state for new game
static void ResetGame(GameBoard* board, Cursor* cursor, SwapAnimation* swapAnim,
                      GravityAnimation* gravityAnim, RiseAnimation* riseAnim,
                      int* comboCount, float* clearTimer, bool* waitingToClear,
                      UIState* ui)
{
    GameBoard_Init(board);
    GameBoard_FillRandom(board);
    Cursor_Init(cursor);
    SwapAnimation_Init(swapAnim);
    GravityAnimation_Init(gravityAnim);
    RiseAnimation_Init(riseAnim);
    *comboCount = 0;
    *clearTimer = 0.0f;
    *waitingToClear = false;
    ui->displayCombo = 0;
    ui->lastClearCount = 0;
    ui->showingMatch = false;
    // Note: highScore is preserved across games
}

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

    // Game state
    GameState gameState = STATE_MENU;
    int finalScore = 0;  // Score when game ended

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

        // Check for state transitions
        GameState newState = GameState_CheckInput(gameState);
        if (newState != gameState) {
            // Handle state transitions
            if (gameState == STATE_MENU && newState == STATE_PLAYING) {
                // Starting new game from menu
                ResetGame(&board, &cursor, &swapAnim, &gravityAnim, &riseAnim,
                         &comboCount, &clearTimer, &waitingToClear, &ui);
            } else if (gameState == STATE_GAME_OVER && newState == STATE_MENU) {
                // Restart requested - will start new game when ENTER pressed
            }
            gameState = newState;
        }

        // Only process game logic when playing
        if (GameState_IsPlaying(gameState)) {
            // Handle cursor movement
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
        }

        // Rendering
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw based on game state
        switch (gameState) {
            case STATE_MENU:
                UI_DrawMenu(WINDOW_WIDTH, WINDOW_HEIGHT);
                break;

            case STATE_PLAYING:
            case STATE_PAUSED:
                // Draw the game board with all animations
                Renderer_DrawBoardWithAnimations(&board, boardX, boardY, &swapAnim, &gravityAnim, &riseAnim);
                Renderer_DrawCursor(cursor.x, cursor.y, boardX, boardY);
                UI_DrawGameplay(&ui, &board, WINDOW_WIDTH);

                if (gameState == STATE_PAUSED) {
                    UI_DrawPause(WINDOW_WIDTH, WINDOW_HEIGHT);
                }
                break;

            case STATE_GAME_OVER:
                // Draw board in background
                Renderer_DrawBoardWithAnimations(&board, boardX, boardY, &swapAnim, &gravityAnim, &riseAnim);
                UI_DrawGameOver(&ui, finalScore, WINDOW_WIDTH, WINDOW_HEIGHT);
                break;
        }

        DrawFPS(WINDOW_WIDTH - 80, 35);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
