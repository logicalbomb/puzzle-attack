#include "dev_settings.h"
#include "raylib.h"
#include <stdio.h>

// Setting labels
static const char* SETTING_LABELS[DEV_SETTINGS_COUNT] = {
    "Disable Auto-Rise"
};

void DevSettings_Init(DevSettings* settings)
{
    settings->disableAutoRise = false;
}

void DevSettingsMenu_Init(DevSettingsMenu* menu)
{
    menu->cursorIndex = 0;
}

void DevSettingsMenu_Draw(const DevSettingsMenu* menu, const DevSettings* settings,
                          int windowWidth, int windowHeight)
{
    // Semi-transparent overlay
    DrawRectangle(0, 0, windowWidth, windowHeight, (Color){0, 0, 0, 200});

    // Title
    const char* title = "DEV SETTINGS";
    int titleSize = 36;
    int titleWidth = MeasureText(title, titleSize);
    DrawText(title, (windowWidth - titleWidth) / 2, windowHeight / 6, titleSize, ORANGE);

    // Instructions
    const char* instructions = "UP/DOWN: navigate | SPACE: toggle | D/ESC: close";
    int instrSize = 16;
    int instrWidth = MeasureText(instructions, instrSize);
    DrawText(instructions, (windowWidth - instrWidth) / 2, windowHeight / 6 + 50, instrSize, GRAY);

    // Settings list
    int optionSize = 24;
    int startY = windowHeight / 3;
    int spacing = 40;
    int centerX = windowWidth / 2;

    for (int i = 0; i < DEV_SETTINGS_COUNT; i++) {
        // Get current value for this setting
        bool value = false;
        switch (i) {
            case 0: value = settings->disableAutoRise; break;
        }

        const char* stateStr = value ? "ON" : "OFF";
        const char* label = SETTING_LABELS[i];

        // Calculate positioning (center the full "Label: STATE" string)
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "%s: %s", label, stateStr);
        int textWidth = MeasureText(buffer, optionSize);
        int textX = centerX - textWidth / 2;
        int textY = startY + i * spacing;

        // Build label with colon
        char labelWithColon[64];
        snprintf(labelWithColon, sizeof(labelWithColon), "%s: ", label);
        int labelWidth = MeasureText(labelWithColon, optionSize);

        // Colors
        Color labelColor = (i == menu->cursorIndex) ? YELLOW : WHITE;
        Color valueColor = value ? GREEN : RED;

        // Draw cursor indicator if selected
        if (i == menu->cursorIndex) {
            DrawText(">", textX - 30, textY, optionSize, YELLOW);
        }

        // Draw label and state separately
        DrawText(labelWithColon, textX, textY, optionSize, labelColor);
        DrawText(stateStr, textX + labelWidth, textY, optionSize, valueColor);
    }
}
