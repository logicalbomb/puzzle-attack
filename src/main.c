#include "raylib.h"
#include "game_board.h"
#include "game_logic.h"
#include "match_detection.h"
#include "physics.h"
#include "renderer.h"
#include "input.h"
#include "ui.h"
#include "game_state.h"
#include "dev_settings.h"

// Game timing constants
static const float CLEAR_DELAY = 0.3f;
static const float GAME_OVER_COUNTDOWN = 5.0f;
static const float MATCH_PAUSE_DURATION = 0.2f;
static const float AUTO_RISE_INTERVAL = 7.0f;

// Game context - holds all game state
typedef struct {
    // Core game objects
    GameBoard board;
    Cursor cursor;

    // Animations
    SwapAnimation swapAnim;
    GravityAnimation gravityAnim;
    RiseAnimation riseAnim;

    // Match/clear state
    int comboCount;
    float clearTimer;
    bool waitingToClear;

    // Timers
    float gameOverTimer;
    float matchPauseTimer;
    float autoRiseTimer;

    // UI
    UIState ui;
    int finalScore;

    // State machine
    GameState state;
    GameState previousState;  // For returning from dev menu

    // Developer settings
    DevSettings devSettings;
    DevSettingsMenu devMenu;

    // Display
    int boardX;
    int boardY;
} Game;

// Forward declarations
static void Game_Init(Game* game);
static void Game_ResetPlay(Game* game);
static void Game_Update(Game* game, float deltaTime);
static void Game_Render(Game* game);

static void Update_Menu(Game* game);
static void Update_Playing(Game* game, float deltaTime);
static void Update_Paused(Game* game);
static void Update_GameOver(Game* game);
static void Update_DevMenu(Game* game);

static void Render_Menu(Game* game);
static void Render_Playing(Game* game);
static void Render_Paused(Game* game);
static void Render_GameOver(Game* game);
static void Render_DevMenu(Game* game);

// Initialize game to starting state
static void Game_Init(Game* game)
{
    GameBoard_Init(&game->board);
    GameBoard_FillRandom(&game->board);
    Cursor_Init(&game->cursor);
    SwapAnimation_Init(&game->swapAnim);
    GravityAnimation_Init(&game->gravityAnim);
    RiseAnimation_Init(&game->riseAnim);

    game->comboCount = 0;
    game->clearTimer = 0.0f;
    game->waitingToClear = false;
    game->gameOverTimer = 0.0f;
    game->matchPauseTimer = 0.0f;
    game->autoRiseTimer = AUTO_RISE_INTERVAL;

    UI_Init(&game->ui);
    game->finalScore = 0;
    game->state = STATE_MENU;
    game->previousState = STATE_MENU;

    DevSettings_Init(&game->devSettings);
    DevSettingsMenu_Init(&game->devMenu);

    game->boardX = Renderer_GetCenteredOffsetX();
    game->boardY = Renderer_GetCenteredOffsetY();
}

// Reset for new game (preserves high score and dev settings)
static void Game_ResetPlay(Game* game)
{
    GameBoard_Init(&game->board);
    GameBoard_FillRandom(&game->board);
    Cursor_Init(&game->cursor);
    SwapAnimation_Init(&game->swapAnim);
    GravityAnimation_Init(&game->gravityAnim);
    RiseAnimation_Init(&game->riseAnim);

    game->comboCount = 0;
    game->clearTimer = 0.0f;
    game->waitingToClear = false;
    game->gameOverTimer = 0.0f;
    game->matchPauseTimer = 0.0f;
    game->autoRiseTimer = AUTO_RISE_INTERVAL;

    game->ui.displayCombo = 0;
    game->ui.lastClearCount = 0;
    game->ui.showingMatch = false;
}

// Main update dispatcher
static void Game_Update(Game* game, float deltaTime)
{
    switch (game->state) {
        case STATE_MENU:
            Update_Menu(game);
            break;
        case STATE_PLAYING:
            Update_Playing(game, deltaTime);
            break;
        case STATE_PAUSED:
            Update_Paused(game);
            break;
        case STATE_GAME_OVER:
            Update_GameOver(game);
            break;
        case STATE_DEV_MENU:
            Update_DevMenu(game);
            break;
    }
}

// Main render dispatcher
static void Game_Render(Game* game)
{
    BeginDrawing();
    ClearBackground(BLACK);

    switch (game->state) {
        case STATE_MENU:
            Render_Menu(game);
            break;
        case STATE_PLAYING:
            Render_Playing(game);
            break;
        case STATE_PAUSED:
            Render_Paused(game);
            break;
        case STATE_GAME_OVER:
            Render_GameOver(game);
            break;
        case STATE_DEV_MENU:
            Render_DevMenu(game);
            break;
    }

    // Debug UI
    DrawFPS(WINDOW_WIDTH - 80, 35);

    EndDrawing();
}

// Menu state
static void Update_Menu(Game* game)
{
    if (IsKeyPressed(KEY_D)) {
        game->previousState = STATE_MENU;
        game->state = STATE_DEV_MENU;
        return;
    }

    if (IsKeyPressed(KEY_ENTER)) {
        Game_ResetPlay(game);
        game->state = STATE_PLAYING;
    }
}

static void Render_Menu(Game* game)
{
    (void)game;  // unused
    UI_DrawMenu(WINDOW_WIDTH, WINDOW_HEIGHT);
}

// Playing state
static void Update_Playing(Game* game, float deltaTime)
{
    // Pause check
    if (IsKeyPressed(KEY_P)) {
        game->state = STATE_PAUSED;
        return;
    }

    // Handle cursor movement
    Cursor_HandleInput(&game->cursor);

    // Handle swap input
    bool canSwap = !game->swapAnim.active && !game->riseAnim.active;
    if (canSwap && Input_SwapPressed()) {
        bool leftFalling = GravityAnimation_IsBlockFalling(&game->gravityAnim, game->cursor.x, game->cursor.y);
        bool rightFalling = GravityAnimation_IsBlockFalling(&game->gravityAnim, game->cursor.x + 1, game->cursor.y);

        if (!leftFalling && !rightFalling) {
            if (SwapBlocks(&game->board, game->cursor.x, game->cursor.y)) {
                SwapAnimation_Start(&game->swapAnim, game->cursor.x, game->cursor.y);
            }
        }
    }

    // Handle manual raise input
    bool animating = game->swapAnim.active || game->gravityAnim.active ||
                     game->riseAnim.active || game->waitingToClear;
    bool inDanger = game->gameOverTimer > 0.0f;
    if (!animating && !inDanger && Input_RaisePressed()) {
        RaiseBoard(&game->board, &game->riseAnim);
    }

    // Update animations
    bool swapCompleted = SwapAnimation_Update(&game->swapAnim, deltaTime);
    bool gravityCompleted = GravityAnimation_Update(&game->gravityAnim, deltaTime);
    bool riseCompleted = RiseAnimation_Update(&game->riseAnim, deltaTime);

    // Check for matches after swap
    if (swapCompleted) {
        int matchCount = DetectMatches(&game->board);
        if (matchCount > 0) {
            game->comboCount = 1;
            game->ui.displayCombo = 1;
            game->ui.lastClearCount = matchCount;
            game->ui.showingMatch = true;
            game->waitingToClear = true;
            game->clearTimer = CLEAR_DELAY;
            game->matchPauseTimer = MATCH_PAUSE_DURATION;
        } else {
            ApplyGravity(&game->board, &game->gravityAnim);
        }
    }

    // Check for cascade matches after gravity
    if (gravityCompleted) {
        int matchCount = DetectMatches(&game->board);
        if (matchCount > 0) {
            game->comboCount++;
            game->ui.displayCombo = game->comboCount;
            game->ui.lastClearCount = matchCount;
            game->ui.showingMatch = true;
            game->waitingToClear = true;
            game->clearTimer = CLEAR_DELAY;
            game->matchPauseTimer = MATCH_PAUSE_DURATION;
        } else {
            game->comboCount = 0;
        }
    }

    // Check for matches after rise
    if (riseCompleted) {
        int matchCount = DetectMatches(&game->board);
        if (matchCount > 0) {
            game->comboCount = 1;
            game->ui.displayCombo = 1;
            game->ui.lastClearCount = matchCount;
            game->ui.showingMatch = true;
            game->waitingToClear = true;
            game->clearTimer = CLEAR_DELAY;
            game->matchPauseTimer = MATCH_PAUSE_DURATION;
        }
    }

    // Clear timer
    if (game->waitingToClear) {
        game->clearTimer -= deltaTime;
        if (game->clearTimer <= 0.0f) {
            game->ui.lastClearCount = ClearMatches(&game->board, game->comboCount);
            game->ui.showingMatch = false;
            game->waitingToClear = false;
            UI_UpdateHighScore(&game->ui, game->board.score);
            ApplyGravity(&game->board, &game->gravityAnim);
        }
    }

    // Game over countdown
    if (GameBoard_IsTopRowFilled(&game->board)) {
        if (game->gameOverTimer <= 0.0f) {
            game->gameOverTimer = GAME_OVER_COUNTDOWN;
        }

        if (game->matchPauseTimer > 0.0f) {
            game->matchPauseTimer -= deltaTime;
        } else {
            game->gameOverTimer -= deltaTime;
            if (game->gameOverTimer <= 0.0f) {
                game->finalScore = game->board.score;
                game->state = STATE_GAME_OVER;
                return;
            }
        }
    } else {
        game->gameOverTimer = 0.0f;
        game->matchPauseTimer = 0.0f;
    }

    // Auto-rise
    if (!game->devSettings.disableAutoRise) {
        bool canAutoRise = !game->swapAnim.active && !game->gravityAnim.active &&
                           !game->riseAnim.active && !game->waitingToClear;
        if (canAutoRise && game->matchPauseTimer <= 0.0f) {
            game->autoRiseTimer -= deltaTime;
            if (game->autoRiseTimer <= 0.0f) {
                RaiseBoard(&game->board, &game->riseAnim);
                game->autoRiseTimer = AUTO_RISE_INTERVAL;
            }
        }
    }
}

static void Render_Playing(Game* game)
{
    // Calculate danger level
    DangerLevel danger = DANGER_NONE;
    if (game->gameOverTimer > 0.0f) {
        danger = (game->gameOverTimer <= 2.0f) ? DANGER_CRITICAL : DANGER_WARNING;
    }

    Renderer_DrawBoardWithAnimations(&game->board, game->boardX, game->boardY,
                                      &game->swapAnim, &game->gravityAnim, &game->riseAnim);
    Renderer_DrawCursor(game->cursor.x, game->cursor.y, game->boardX, game->boardY);
    UI_DrawGameplay(&game->ui, &game->board, WINDOW_WIDTH, danger);
}

// Paused state
static void Update_Paused(Game* game)
{
    if (IsKeyPressed(KEY_D)) {
        game->previousState = STATE_PAUSED;
        game->state = STATE_DEV_MENU;
        return;
    }

    if (IsKeyPressed(KEY_P)) {
        game->state = STATE_PLAYING;
    } else if (IsKeyPressed(KEY_M)) {
        game->state = STATE_MENU;
    }
}

static void Render_Paused(Game* game)
{
    // Draw game in background
    Render_Playing(game);
    // Draw pause overlay
    UI_DrawPause(WINDOW_WIDTH, WINDOW_HEIGHT);
}

// Game over state
static void Update_GameOver(Game* game)
{
    if (IsKeyPressed(KEY_D)) {
        game->previousState = STATE_GAME_OVER;
        game->state = STATE_DEV_MENU;
        return;
    }

    if (IsKeyPressed(KEY_R)) {
        Game_ResetPlay(game);
        game->state = STATE_PLAYING;
    } else if (IsKeyPressed(KEY_M)) {
        game->state = STATE_MENU;
    }
}

static void Render_GameOver(Game* game)
{
    // Draw board in background
    Renderer_DrawBoardWithAnimations(&game->board, game->boardX, game->boardY,
                                      &game->swapAnim, &game->gravityAnim, &game->riseAnim);
    UI_DrawGameOver(&game->ui, game->finalScore, WINDOW_WIDTH, WINDOW_HEIGHT);
}

// Dev menu state
static void Update_DevMenu(Game* game)
{
    // Navigate with arrow keys
    if (IsKeyPressed(KEY_UP)) {
        game->devMenu.cursorIndex--;
        if (game->devMenu.cursorIndex < 0) {
            game->devMenu.cursorIndex = DEV_SETTINGS_COUNT - 1;
        }
    }
    if (IsKeyPressed(KEY_DOWN)) {
        game->devMenu.cursorIndex++;
        if (game->devMenu.cursorIndex >= DEV_SETTINGS_COUNT) {
            game->devMenu.cursorIndex = 0;
        }
    }

    // Toggle selected option
    if (IsKeyPressed(KEY_SPACE)) {
        switch (game->devMenu.cursorIndex) {
            case 0:
                game->devSettings.disableAutoRise = !game->devSettings.disableAutoRise;
                break;
        }
    }

    // Close dev menu
    if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_ESCAPE)) {
        game->state = game->previousState;
    }
}

static void Render_DevMenu(Game* game)
{
    // Draw previous state in background
    switch (game->previousState) {
        case STATE_MENU:
            Render_Menu(game);
            break;
        case STATE_PLAYING:
            Render_Playing(game);
            break;
        case STATE_PAUSED:
            Render_Paused(game);
            break;
        case STATE_GAME_OVER:
            Render_GameOver(game);
            break;
        case STATE_DEV_MENU:
            break;  // shouldn't happen
    }

    // Draw dev menu overlay
    DevSettingsMenu_Draw(&game->devMenu, &game->devSettings, WINDOW_WIDTH, WINDOW_HEIGHT);
}

// Main entry point
int main(void)
{
    Game game;
    Game_Init(&game);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Puzzle Attack");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        Game_Update(&game, deltaTime);
        Game_Render(&game);
    }

    CloseWindow();
    return 0;
}
