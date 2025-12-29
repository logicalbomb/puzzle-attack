#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdbool.h>

// Client game states (server will have different states)
typedef enum {
    STATE_MENU,
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_GAME_OVER
} GameState;

// Check for state transition inputs (client-only, uses raylib)
// Returns new state based on current state and input
GameState GameState_CheckInput(GameState current);

// Check if game logic should update (only in PLAYING state)
bool GameState_IsPlaying(GameState state);

#endif // GAME_STATE_H
