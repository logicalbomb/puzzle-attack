#ifndef UI_H
#define UI_H

#include "game_board.h"

// UI state for tracking display values
typedef struct {
    int highScore;
    int displayCombo;      // Current combo to display
    int lastClearCount;    // Last number of blocks cleared
    bool showingMatch;     // Currently showing match feedback
} UIState;

// Initialize UI state
void UI_Init(UIState* ui);

// Update high score if current score is higher
void UI_UpdateHighScore(UIState* ui, int currentScore);

// Danger level for status indicator
typedef enum {
    DANGER_NONE,    // Green - no danger
    DANGER_WARNING, // Yellow - countdown started (first 3 seconds)
    DANGER_CRITICAL // Red - final 2 seconds
} DangerLevel;

// Draw gameplay UI elements (score, combo, etc.)
void UI_DrawGameplay(const UIState* ui, const GameBoard* board, int windowWidth, DangerLevel danger);

// Draw menu screen
void UI_DrawMenu(int windowWidth, int windowHeight);

// Draw pause overlay
void UI_DrawPause(int windowWidth, int windowHeight);

// Draw game over screen
void UI_DrawGameOver(const UIState* ui, int finalScore, int windowWidth, int windowHeight);

#endif // UI_H
