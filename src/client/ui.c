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

void UI_DrawGameplay(const UIState* ui, const GameBoard* board, int windowWidth)
{
    // Title (top-left)
    DrawText("Puzzle Attack", 10, 10, 20, WHITE);

    // Controls hint
    DrawText("Arrows: move | SPACE: swap | SHIFT: raise | P: pause", 10, 35, 16, GRAY);

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

void UI_DrawMenu(int windowWidth, int windowHeight)
{
    // Title
    const char* title = "PUZZLE ATTACK";
    int titleSize = 48;
    int titleWidth = MeasureText(title, titleSize);
    DrawText(title, (windowWidth - titleWidth) / 2, windowHeight / 3, titleSize, WHITE);

    // Start prompt
    const char* prompt = "Press ENTER to start";
    int promptSize = 24;
    int promptWidth = MeasureText(prompt, promptSize);
    DrawText(prompt, (windowWidth - promptWidth) / 2, windowHeight / 2, promptSize, GRAY);

    // Controls hint
    const char* controls = "Arrows: move | SPACE: swap | SHIFT: raise | P: pause";
    int controlsSize = 16;
    int controlsWidth = MeasureText(controls, controlsSize);
    DrawText(controls, (windowWidth - controlsWidth) / 2, windowHeight * 2 / 3, controlsSize, DARKGRAY);
}

void UI_DrawPause(int windowWidth, int windowHeight)
{
    // Semi-transparent overlay
    DrawRectangle(0, 0, windowWidth, windowHeight, (Color){0, 0, 0, 180});

    // Pause text
    const char* pauseText = "PAUSED";
    int pauseSize = 48;
    int pauseWidth = MeasureText(pauseText, pauseSize);
    DrawText(pauseText, (windowWidth - pauseWidth) / 2, windowHeight / 3, pauseSize, WHITE);

    // Resume prompt
    const char* prompt = "Press P to resume";
    int promptSize = 24;
    int promptWidth = MeasureText(prompt, promptSize);
    DrawText(prompt, (windowWidth - promptWidth) / 2, windowHeight / 2, promptSize, GRAY);
}

void UI_DrawGameOver(const UIState* ui, int finalScore, int windowWidth, int windowHeight)
{
    // Semi-transparent overlay
    DrawRectangle(0, 0, windowWidth, windowHeight, (Color){0, 0, 0, 200});

    // Game over text
    const char* gameOverText = "GAME OVER";
    int gameOverSize = 48;
    int gameOverWidth = MeasureText(gameOverText, gameOverSize);
    DrawText(gameOverText, (windowWidth - gameOverWidth) / 2, windowHeight / 4, gameOverSize, RED);

    // Final score
    const char* scoreText = TextFormat("Final Score: %d", finalScore);
    int scoreSize = 32;
    int scoreWidth = MeasureText(scoreText, scoreSize);
    DrawText(scoreText, (windowWidth - scoreWidth) / 2, windowHeight / 2 - 40, scoreSize, YELLOW);

    // High score
    const char* highText = TextFormat("High Score: %d", ui->highScore);
    int highSize = 24;
    int highWidth = MeasureText(highText, highSize);
    DrawText(highText, (windowWidth - highWidth) / 2, windowHeight / 2 + 10, highSize, GOLD);

    // Restart prompt
    const char* prompt = "Press R to restart";
    int promptSize = 24;
    int promptWidth = MeasureText(prompt, promptSize);
    DrawText(prompt, (windowWidth - promptWidth) / 2, windowHeight * 2 / 3, promptSize, GRAY);
}
