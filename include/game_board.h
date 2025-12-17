#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include <stdint.h>
#include <stdbool.h>

// Board dimensions
#define BOARD_WIDTH  6
#define BOARD_HEIGHT 12

// Block types (lower byte - bitfield values)
typedef enum {
    BLOCK_EMPTY  = 0x00,
    BLOCK_RED    = 0x01,
    BLOCK_BLUE   = 0x02,
    BLOCK_GREEN  = 0x04,
    BLOCK_YELLOW = 0x08,
    BLOCK_PURPLE = 0x10
} BlockType;

// Block states (upper byte)
typedef enum {
    STATE_NORMAL  = 0x00,
    STATE_FALLING = 0x01,
    STATE_MATCHED = 0x02,
    STATE_LOCKED  = 0x04
} BlockState;

// Helper macros for cell manipulation
#define BLOCK_TYPE(cell)  ((BlockType)((cell) & 0xFF))
#define BLOCK_STATE(cell) ((BlockState)(((cell) >> 8) & 0xFF))
#define MAKE_BLOCK(type, state) ((uint16_t)(((state) << 8) | (type)))

// Grid indexing
#define BOARD_SIZE (BOARD_WIDTH * BOARD_HEIGHT)
#define GRID_INDEX(x, y) ((y) * BOARD_WIDTH + (x))

// Game board structure
typedef struct {
    uint16_t grid[BOARD_SIZE];
    int score;
    int combo;
} GameBoard;

// Function declarations
void GameBoard_Init(GameBoard* board);
void GameBoard_Clear(GameBoard* board);
uint16_t GameBoard_GetCell(const GameBoard* board, int x, int y);
void GameBoard_SetCell(GameBoard* board, int x, int y, uint16_t value);
void GameBoard_SetBlockType(GameBoard* board, int x, int y, BlockType type);
void GameBoard_SetBlockState(GameBoard* board, int x, int y, BlockState state);
bool GameBoard_IsValidPosition(int x, int y);

#endif // GAME_BOARD_H
