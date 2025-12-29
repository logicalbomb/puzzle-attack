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

// Draw all UI elements
// boardX: x position of game board (for positioning relative elements)
void UI_Draw(const UIState* ui, const GameBoard* board, int windowWidth);

#endif // UI_H
