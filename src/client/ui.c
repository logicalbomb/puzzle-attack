#include "ui.h"
#include "raylib.h"

void UI_Init(UIState* ui)
{
    ui->highScore = 0;
    ui->displayCombo = 0;
    ui->lastClearCount = 0;
    ui->showingMatch = false;
}

void UI_UpdateHighScore(UIState* ui, int currentScore)
{
    if (currentScore > ui->highScore) {
        ui->highScore = currentScore;
    }
}

void UI_Draw(const UIState* ui, const GameBoard* board, int windowWidth)
{
    // Title (top-left)
    DrawText("Puzzle Attack", 10, 10, 20, WHITE);

    // Controls hint
    DrawText("Arrow keys: move | SPACE: swap | SHIFT: raise", 10, 35, 16, GRAY);

    // Current score (top-left, below controls)
    DrawText(TextFormat("Score: %d", board->score), 10, 60, 20, YELLOW);

    // High score (top-right)
    const char* highScoreText = TextFormat("High: %d", ui->highScore);
    int highScoreWidth = MeasureText(highScoreText, 20);
    DrawText(highScoreText, windowWidth - highScoreWidth - 10, 10, 20, GOLD);

    // Match/clear feedback (below score)
    if (ui->showingMatch) {
        DrawText(TextFormat("Matched: %d blocks!", ui->lastClearCount), 10, 85, 16, GREEN);
    } else if (ui->lastClearCount > 0) {
        DrawText(TextFormat("Cleared: %d blocks", ui->lastClearCount), 10, 85, 16, LIME);
    }

    // Combo counter (top-center, only show for cascades 2+)
    if (ui->displayCombo >= 2) {
        const char* comboText = TextFormat("%dx COMBO!", ui->displayCombo);
        int comboWidth = MeasureText(comboText, 28);
        DrawText(comboText, (windowWidth - comboWidth) / 2, 60, 28, ORANGE);
    }
}
