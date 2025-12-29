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

// Game over countdown settings
static const float GAME_OVER_COUNTDOWN = 5.0f;  // Total countdown time
static const float MATCH_PAUSE_DURATION = 0.2f; // Pause countdown when match detected

// Auto-rise settings
static const float AUTO_RISE_INTERVAL = 7.0f;  // Time between auto-rises

// Helper to reset game state for new game
static void ResetGame(GameBoard* board, Cursor* cursor, SwapAnimation* swapAnim,
                      GravityAnimation* gravityAnim, RiseAnimation* riseAnim,
                      int* comboCount, float* clearTimer, bool* waitingToClear,
                      float* gameOverTimer, float* matchPauseTimer,
                      float* autoRiseTimer, UIState* ui)
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
    *gameOverTimer = 0.0f;
    *matchPauseTimer = 0.0f;
    *autoRiseTimer = AUTO_RISE_INTERVAL;
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

    // Game over countdown
    float gameOverTimer = 0.0f;   // Countdown to game over (0 = not counting)
    float matchPauseTimer = 0.0f; // Pause countdown when match detected

    // Auto-rise timer
    float autoRiseTimer = AUTO_RISE_INTERVAL;

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
            // Handle state transitions that need game reset
            if (newState == STATE_PLAYING &&
                (gameState == STATE_MENU || gameState == STATE_GAME_OVER)) {
                // Starting new game from menu or restart from game over
                ResetGame(&board, &cursor, &swapAnim, &gravityAnim, &riseAnim,
                         &comboCount, &clearTimer, &waitingToClear,
                         &gameOverTimer, &matchPauseTimer, &autoRiseTimer, &ui);
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

            // Handle raise input (only when not animating and not in danger)
            bool animating = swapAnim.active || gravityAnim.active || riseAnim.active || waitingToClear;
            bool inDanger = gameOverTimer > 0.0f;
            if (!animating && !inDanger && Input_RaisePressed()) {
                RaiseBoard(&board, &riseAnim);
                // Note: Can't raise if top row filled, but game over is now handled by countdown
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
                    matchPauseTimer = MATCH_PAUSE_DURATION;  // Pause countdown
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
                    matchPauseTimer = MATCH_PAUSE_DURATION;  // Pause countdown
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
                    matchPauseTimer = MATCH_PAUSE_DURATION;  // Pause countdown
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

            // Game over countdown logic
            if (GameBoard_IsTopRowFilled(&board)) {
                // Start countdown if not already running
                if (gameOverTimer <= 0.0f) {
                    gameOverTimer = GAME_OVER_COUNTDOWN;
                }

                // Update match pause timer
                if (matchPauseTimer > 0.0f) {
                    matchPauseTimer -= deltaTime;
                } else {
                    // Decrement countdown
                    gameOverTimer -= deltaTime;
                    if (gameOverTimer <= 0.0f) {
                        // Game over!
                        finalScore = board.score;
                        gameState = STATE_GAME_OVER;
                    }
                }
            } else {
                // Top row clear - reset countdown
                gameOverTimer = 0.0f;
                matchPauseTimer = 0.0f;
            }

            // Auto-rise logic (works even during game over countdown)
            bool canAutoRise = !swapAnim.active && !gravityAnim.active && !riseAnim.active && !waitingToClear;
            if (canAutoRise) {
                // Pause auto-rise timer when match pause is active
                if (matchPauseTimer <= 0.0f) {
                    autoRiseTimer -= deltaTime;
                    if (autoRiseTimer <= 0.0f) {
                        RaiseBoard(&board, &riseAnim);
                        autoRiseTimer = AUTO_RISE_INTERVAL;
                    }
                }
            }
        }

        // Calculate danger level for UI
        DangerLevel danger = DANGER_NONE;
        if (gameOverTimer > 0.0f) {
            if (gameOverTimer <= 2.0f) {
                danger = DANGER_CRITICAL;
            } else {
                danger = DANGER_WARNING;
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
                UI_DrawGameplay(&ui, &board, WINDOW_WIDTH, danger);

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
