#ifndef RENDERER_H
#define RENDERER_H

#include "game_board.h"

// Rendering constants
#define BLOCK_SIZE 48
#define GRID_LINE_WIDTH 2

// Board dimensions in pixels
#define BOARD_PIXEL_WIDTH  (BOARD_WIDTH * BLOCK_SIZE)
#define BOARD_PIXEL_HEIGHT (BOARD_HEIGHT * BLOCK_SIZE)

// Window dimensions
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 650

// Render the game board at the specified offset
void Renderer_DrawBoard(const GameBoard* board, int offsetX, int offsetY);

// Render a single block at grid position
void Renderer_DrawBlock(BlockType type, int gridX, int gridY, int offsetX, int offsetY);

// Calculate offset to center board in window
int Renderer_GetCenteredOffsetX(void);
int Renderer_GetCenteredOffsetY(void);

#endif // RENDERER_H
