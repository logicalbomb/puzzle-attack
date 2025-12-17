#include "game_board.h"
#include <stdlib.h>
#include <time.h>

// Lookup table to convert index (0-4) to BlockType
static const BlockType BLOCK_TYPES[BLOCK_TYPE_COUNT] = {
    BLOCK_RED,
    BLOCK_BLUE,
    BLOCK_GREEN,
    BLOCK_YELLOW,
    BLOCK_PURPLE
};

// Get a random block type
static BlockType GetRandomBlockType(void)
{
    int index = rand() % BLOCK_TYPE_COUNT;
    return BLOCK_TYPES[index];
}

// Check if placing a block at (x, y) would create a horizontal match
static bool WouldMatchHorizontal(const GameBoard* board, int x, int y, BlockType type)
{
    if (x < 2) return false;

    BlockType left1 = BLOCK_TYPE(GameBoard_GetCell(board, x - 1, y));
    BlockType left2 = BLOCK_TYPE(GameBoard_GetCell(board, x - 2, y));

    return (left1 == type && left2 == type);
}

// Check if placing a block at (x, y) would create a vertical match
static bool WouldMatchVertical(const GameBoard* board, int x, int y, BlockType type)
{
    if (y < 2) return false;

    BlockType up1 = BLOCK_TYPE(GameBoard_GetCell(board, x, y - 1));
    BlockType up2 = BLOCK_TYPE(GameBoard_GetCell(board, x, y - 2));

    return (up1 == type && up2 == type);
}

// Check if placing a block would create any match
static bool WouldCreateMatch(const GameBoard* board, int x, int y, BlockType type)
{
    return WouldMatchHorizontal(board, x, y, type) ||
           WouldMatchVertical(board, x, y, type);
}

void GameBoard_FillRandom(GameBoard* board)
{
    static bool seeded = false;
    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = true;
    }

    GameBoard_Clear(board);

    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            BlockType type;
            int retries = 0;
            const int maxRetries = 10;

            do {
                type = GetRandomBlockType();
                retries++;
            } while (WouldCreateMatch(board, x, y, type) && retries < maxRetries);

            GameBoard_SetCell(board, x, y, MAKE_BLOCK(type, STATE_NORMAL));
        }
    }
}
