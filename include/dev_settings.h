#ifndef DEV_SETTINGS_H
#define DEV_SETTINGS_H

#include <stdbool.h>
#include <stdint.h>

// Developer settings (persist during session)
typedef struct {
    bool disableAutoRise;
    bool showDebugUI;
    uint64_t gameSeed;  // Seed for next game (0 = random)
    bool dumpLogRequested;  // Set to true when dump is triggered
} DevSettings;

// Dev settings menu state
typedef struct {
    int cursorIndex;   // Selected option (0-indexed)
} DevSettingsMenu;

// Number of toggle settings
#define DEV_SETTINGS_COUNT 2

// Number of action buttons
#define DEV_ACTIONS_COUNT 1

// Initialize dev settings to defaults
void DevSettings_Init(DevSettings* settings);

// Initialize dev settings menu
void DevSettingsMenu_Init(DevSettingsMenu* menu);

// Draw dev settings menu overlay
void DevSettingsMenu_Draw(const DevSettingsMenu* menu, const DevSettings* settings,
                          int windowWidth, int windowHeight);

#endif // DEV_SETTINGS_H
