#include "dev_settings.h"
#include "raylib.h"
#include <stdio.h>

// Setting labels (toggles)
static const char* SETTING_LABELS[DEV_SETTINGS_COUNT] = {
    "Disable Auto-Rise",
    "Show Debug UI"
};

// Action labels
static const char* ACTION_LABELS[DEV_ACTIONS_COUNT] = {
    "Dump Game Log"
};

void DevSettings_Init(DevSettings* settings)
{
    settings->disableAutoRise = false;
    settings->showDebugUI = true;
    settings->gameSeed = 0;  // 0 = random seed
    settings->dumpLogRequested = false;
}

void DevSettingsMenu_Init(DevSettingsMenu* menu)
{
    menu->cursorIndex = 0;
}

void DevSettingsMenu_Draw(const DevSettingsMenu* menu, const DevSettings* settings,
                          int windowWidth, int windowHeight)
{
    (void)settings;  // Used for toggle values

    // Semi-transparent overlay
    DrawRectangle(0, 0, windowWidth, windowHeight, (Color){0, 0, 0, 200});

    // Title
    const char* title = "DEV SETTINGS";
    int titleSize = 36;
    int titleWidth = MeasureText(title, titleSize);
    DrawText(title, (windowWidth - titleWidth) / 2, windowHeight / 6, titleSize, ORANGE);

    // Instructions
    const char* instructions = "UP/DOWN: navigate | SPACE: toggle/trigger | D/ESC: close";
    int instrSize = 16;
    int instrWidth = MeasureText(instructions, instrSize);
    DrawText(instructions, (windowWidth - instrWidth) / 2, windowHeight / 6 + 50, instrSize, GRAY);

    // Settings list
    int optionSize = 24;
    int startY = windowHeight / 3;
    int spacing = 40;
    int centerX = windowWidth / 2;
    int totalItems = DEV_SETTINGS_COUNT + DEV_ACTIONS_COUNT;

    for (int i = 0; i < totalItems; i++) {
        int textY = startY + i * spacing;
        Color labelColor = (i == menu->cursorIndex) ? YELLOW : WHITE;

        if (i < DEV_SETTINGS_COUNT) {
            // Toggle setting
            bool value = false;
            switch (i) {
                case 0: value = settings->disableAutoRise; break;
                case 1: value = settings->showDebugUI; break;
            }

            const char* stateStr = value ? "ON" : "OFF";
            const char* label = SETTING_LABELS[i];

            char buffer[64];
            snprintf(buffer, sizeof(buffer), "%s: %s", label, stateStr);
            int textWidth = MeasureText(buffer, optionSize);
            int textX = centerX - textWidth / 2;

            char labelWithColon[64];
            snprintf(labelWithColon, sizeof(labelWithColon), "%s: ", label);
            int labelWidth = MeasureText(labelWithColon, optionSize);

            Color valueColor = value ? GREEN : RED;

            if (i == menu->cursorIndex) {
                DrawText(">", textX - 30, textY, optionSize, YELLOW);
            }

            DrawText(labelWithColon, textX, textY, optionSize, labelColor);
            DrawText(stateStr, textX + labelWidth, textY, optionSize, valueColor);
        } else {
            // Action button
            int actionIndex = i - DEV_SETTINGS_COUNT;
            const char* label = ACTION_LABELS[actionIndex];

            char buffer[64];
            snprintf(buffer, sizeof(buffer), "[%s]", label);
            int textWidth = MeasureText(buffer, optionSize);
            int textX = centerX - textWidth / 2;

            if (i == menu->cursorIndex) {
                DrawText(">", textX - 30, textY, optionSize, YELLOW);
            }

            DrawText(buffer, textX, textY, optionSize, labelColor);
        }
    }
}
