#include "game_board.h"
#include <string.h>

void GameBoard_Init(GameBoard* board)
{
    GameBoard_Clear(board);
    board->score = 0;
    board->combo = 0;
}

void GameBoard_Clear(GameBoard* board)
{
    memset(board->grid, 0, sizeof(board->grid));
}

uint16_t GameBoard_GetCell(const GameBoard* board, int x, int y)
{
    if (!GameBoard_IsValidPosition(x, y)) {
        return MAKE_BLOCK(BLOCK_EMPTY, STATE_NORMAL);
    }
    return board->grid[GRID_INDEX(x, y)];
}

void GameBoard_SetCell(GameBoard* board, int x, int y, uint16_t value)
{
    if (GameBoard_IsValidPosition(x, y)) {
        board->grid[GRID_INDEX(x, y)] = value;
    }
}

void GameBoard_SetBlockType(GameBoard* board, int x, int y, BlockType type)
{
    if (GameBoard_IsValidPosition(x, y)) {
        int idx = GRID_INDEX(x, y);
        BlockState state = BLOCK_STATE(board->grid[idx]);
        board->grid[idx] = MAKE_BLOCK(type, state);
    }
}

void GameBoard_SetBlockState(GameBoard* board, int x, int y, BlockState state)
{
    if (GameBoard_IsValidPosition(x, y)) {
        int idx = GRID_INDEX(x, y);
        BlockType type = BLOCK_TYPE(board->grid[idx]);
        board->grid[idx] = MAKE_BLOCK(type, state);
    }
}

bool GameBoard_IsValidPosition(int x, int y)
{
    return x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT;
}
