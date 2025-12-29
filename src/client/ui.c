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

void UI_DrawGameplay(const UIState* ui, const GameBoard* board, int windowWidth, DangerLevel danger)
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

    // Danger indicator (below high score)
    Color dangerColor;
    switch (danger) {
        case DANGER_NONE:     dangerColor = GREEN; break;
        case DANGER_WARNING:  dangerColor = YELLOW; break;
        case DANGER_CRITICAL: dangerColor = RED; break;
        default:              dangerColor = GREEN; break;
    }
    DrawRectangle(windowWidth - 50, 35, 40, 20, dangerColor);

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
    DrawText(title, (windowWidth - titleWidth) / 2, windowHeight / 4, titleSize, WHITE);

    // Start prompt
    const char* startPrompt = "ENTER - Start";
    int startSize = 24;
    int startWidth = MeasureText(startPrompt, startSize);
    DrawText(startPrompt, (windowWidth - startWidth) / 2, windowHeight / 2 - 20, startSize, GRAY);

    // Quit prompt
    const char* quitPrompt = "ESC - Quit";
    int quitSize = 24;
    int quitWidth = MeasureText(quitPrompt, quitSize);
    DrawText(quitPrompt, (windowWidth - quitWidth) / 2, windowHeight / 2 + 20, quitSize, GRAY);

    // Controls hint
    const char* controls = "Arrows: move | SPACE: swap | SHIFT: raise | P: pause";
    int controlsSize = 16;
    int controlsWidth = MeasureText(controls, controlsSize);
    DrawText(controls, (windowWidth - controlsWidth) / 2, windowHeight * 3 / 4, controlsSize, DARKGRAY);
}

void UI_DrawPause(int windowWidth, int windowHeight)
{
    // Semi-transparent overlay
    DrawRectangle(0, 0, windowWidth, windowHeight, (Color){0, 0, 0, 180});

    // Pause text
    const char* pauseText = "PAUSED";
    int pauseSize = 48;
    int pauseWidth = MeasureText(pauseText, pauseSize);
    DrawText(pauseText, (windowWidth - pauseWidth) / 2, windowHeight / 4, pauseSize, WHITE);

    // Options
    int optionSize = 24;
    int centerX = windowWidth / 2;
    int startY = windowHeight / 2 - 40;
    int spacing = 40;

    const char* resumeText = "P - Resume";
    DrawText(resumeText, centerX - MeasureText(resumeText, optionSize) / 2, startY, optionSize, GRAY);

    const char* menuText = "M - Menu";
    DrawText(menuText, centerX - MeasureText(menuText, optionSize) / 2, startY + spacing, optionSize, GRAY);

    const char* quitText = "ESC - Quit";
    DrawText(quitText, centerX - MeasureText(quitText, optionSize) / 2, startY + spacing * 2, optionSize, GRAY);
}

void UI_DrawGameOver(const UIState* ui, int finalScore, int windowWidth, int windowHeight)
{
    // Semi-transparent overlay
    DrawRectangle(0, 0, windowWidth, windowHeight, (Color){0, 0, 0, 200});

    // Game over text
    const char* gameOverText = "GAME OVER";
    int gameOverSize = 48;
    int gameOverWidth = MeasureText(gameOverText, gameOverSize);
    DrawText(gameOverText, (windowWidth - gameOverWidth) / 2, windowHeight / 6, gameOverSize, RED);

    // Final score
    const char* scoreText = TextFormat("Final Score: %d", finalScore);
    int scoreSize = 32;
    int scoreWidth = MeasureText(scoreText, scoreSize);
    DrawText(scoreText, (windowWidth - scoreWidth) / 2, windowHeight / 3, scoreSize, YELLOW);

    // High score
    const char* highText = TextFormat("High Score: %d", ui->highScore);
    int highSize = 24;
    int highWidth = MeasureText(highText, highSize);
    DrawText(highText, (windowWidth - highWidth) / 2, windowHeight / 3 + 50, highSize, GOLD);

    // Options
    int optionSize = 24;
    int centerX = windowWidth / 2;
    int startY = windowHeight * 3 / 5;
    int spacing = 36;

    const char* restartText = "R - Restart";
    DrawText(restartText, centerX - MeasureText(restartText, optionSize) / 2, startY, optionSize, GRAY);

    const char* menuText = "M - Menu";
    DrawText(menuText, centerX - MeasureText(menuText, optionSize) / 2, startY + spacing, optionSize, GRAY);

    const char* quitText = "ESC - Quit";
    DrawText(quitText, centerX - MeasureText(quitText, optionSize) / 2, startY + spacing * 2, optionSize, GRAY);
}
