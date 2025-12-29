#ifndef INPUT_H
#define INPUT_H

#include "game_board.h"

// Cursor structure for block selection
typedef struct {
    int x;
    int y;
} Cursor;

// Initialize cursor to default position
void Cursor_Init(Cursor* cursor);

// Handle input and update cursor position
// Returns true if cursor moved
bool Cursor_HandleInput(Cursor* cursor);

// Clamp cursor to valid board position
void Cursor_Clamp(Cursor* cursor);

// Check if swap key (SPACE) was pressed
bool Input_SwapPressed(void);

// Check if raise key (either SHIFT) was pressed
bool Input_RaisePressed(void);

#endif // INPUT_H
