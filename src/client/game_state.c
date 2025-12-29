#include "game_state.h"
#include "raylib.h"

GameState GameState_CheckInput(GameState current)
{
    switch (current) {
        case STATE_MENU:
            if (IsKeyPressed(KEY_ENTER)) {
                return STATE_PLAYING;
            }
            break;

        case STATE_PLAYING:
            if (IsKeyPressed(KEY_P)) {
                return STATE_PAUSED;
            }
            break;

        case STATE_PAUSED:
            if (IsKeyPressed(KEY_P)) {
                return STATE_PLAYING;
            }
            break;

        case STATE_GAME_OVER:
            if (IsKeyPressed(KEY_R)) {
                return STATE_MENU;  // Return to menu, main.c will reset game
            }
            break;
    }

    return current;
}

bool GameState_IsPlaying(GameState state)
{
    return state == STATE_PLAYING;
}
