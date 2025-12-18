#include "renderer.h"
#include "raylib.h"

// Map BlockType to raylib Color
static Color GetBlockColor(BlockType type)
{
    switch (type) {
        case BLOCK_RED:    return RED;
        case BLOCK_BLUE:   return BLUE;
        case BLOCK_GREEN:  return GREEN;
        case BLOCK_YELLOW: return YELLOW;
        case BLOCK_PURPLE: return PURPLE;
        case BLOCK_EMPTY:
        default:           return BLANK;
    }
}

static const int BLOCK_PADDING = 2;

void Renderer_DrawBlockAtPixel(BlockType type, int pixelX, int pixelY)
{
    if (type == BLOCK_EMPTY) {
        return;
    }

    Color color = GetBlockColor(type);

    DrawRectangle(
        pixelX + BLOCK_PADDING,
        pixelY + BLOCK_PADDING,
        BLOCK_SIZE - (BLOCK_PADDING * 2),
        BLOCK_SIZE - (BLOCK_PADDING * 2),
        color
    );
}

// Draw a block with state-based rendering (e.g., matched blocks flash white)
static void DrawBlockWithState(BlockType type, BlockState state, int pixelX, int pixelY)
{
    if (type == BLOCK_EMPTY) {
        return;
    }

    Color color = GetBlockColor(type);

    // Matched blocks get a white overlay
    if (state == STATE_MATCHED) {
        // Draw base color
        DrawRectangle(
            pixelX + BLOCK_PADDING,
            pixelY + BLOCK_PADDING,
            BLOCK_SIZE - (BLOCK_PADDING * 2),
            BLOCK_SIZE - (BLOCK_PADDING * 2),
            color
        );
        // Draw white overlay with transparency
        DrawRectangle(
            pixelX + BLOCK_PADDING,
            pixelY + BLOCK_PADDING,
            BLOCK_SIZE - (BLOCK_PADDING * 2),
            BLOCK_SIZE - (BLOCK_PADDING * 2),
            (Color){ 255, 255, 255, 150 }
        );
        // Draw white border to highlight
        DrawRectangleLines(
            pixelX + BLOCK_PADDING,
            pixelY + BLOCK_PADDING,
            BLOCK_SIZE - (BLOCK_PADDING * 2),
            BLOCK_SIZE - (BLOCK_PADDING * 2),
            WHITE
        );
    } else {
        DrawRectangle(
            pixelX + BLOCK_PADDING,
            pixelY + BLOCK_PADDING,
            BLOCK_SIZE - (BLOCK_PADDING * 2),
            BLOCK_SIZE - (BLOCK_PADDING * 2),
            color
        );
    }
}

void Renderer_DrawBlock(BlockType type, int gridX, int gridY, int offsetX, int offsetY)
{
    int pixelX = offsetX + (gridX * BLOCK_SIZE);
    int pixelY = offsetY + (gridY * BLOCK_SIZE);
    Renderer_DrawBlockAtPixel(type, pixelX, pixelY);
}

void Renderer_DrawBoard(const GameBoard* board, int offsetX, int offsetY)
{
    // Draw background
    DrawRectangle(
        offsetX,
        offsetY,
        BOARD_PIXEL_WIDTH,
        BOARD_PIXEL_HEIGHT,
        DARKGRAY
    );

    // Draw grid lines
    for (int x = 0; x <= BOARD_WIDTH; x++) {
        int lineX = offsetX + (x * BLOCK_SIZE);
        DrawLine(lineX, offsetY, lineX, offsetY + BOARD_PIXEL_HEIGHT, GRAY);
    }
    for (int y = 0; y <= BOARD_HEIGHT; y++) {
        int lineY = offsetY + (y * BLOCK_SIZE);
        DrawLine(offsetX, lineY, offsetX + BOARD_PIXEL_WIDTH, lineY, GRAY);
    }

    // Draw blocks
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            uint16_t cell = board->grid[GRID_INDEX(x, y)];
            BlockType type = BLOCK_TYPE(cell);
            Renderer_DrawBlock(type, x, y, offsetX, offsetY);
        }
    }

    // Draw grid coordinates (for debugging)
    for (int x = 0; x < BOARD_WIDTH; x++) {
        int pixelX = offsetX + (x * BLOCK_SIZE) + (BLOCK_SIZE / 2) - 4;
        int pixelY = offsetY + BOARD_PIXEL_HEIGHT + 5;
        DrawText(TextFormat("%d", x), pixelX, pixelY, 10, LIGHTGRAY);
    }
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        int pixelX = offsetX - 15;
        int pixelY = offsetY + (y * BLOCK_SIZE) + (BLOCK_SIZE / 2) - 5;
        DrawText(TextFormat("%d", y), pixelX, pixelY, 10, LIGHTGRAY);
    }

    // Draw board outline
    DrawRectangleLines(offsetX, offsetY, BOARD_PIXEL_WIDTH, BOARD_PIXEL_HEIGHT, WHITE);
}

void Renderer_DrawBoardWithSwap(const GameBoard* board, int offsetX, int offsetY,
                                 const SwapAnimation* swapAnim)
{
    // Draw background
    DrawRectangle(offsetX, offsetY, BOARD_PIXEL_WIDTH, BOARD_PIXEL_HEIGHT, DARKGRAY);

    // Draw grid lines
    for (int x = 0; x <= BOARD_WIDTH; x++) {
        int lineX = offsetX + (x * BLOCK_SIZE);
        DrawLine(lineX, offsetY, lineX, offsetY + BOARD_PIXEL_HEIGHT, GRAY);
    }
    for (int y = 0; y <= BOARD_HEIGHT; y++) {
        int lineY = offsetY + (y * BLOCK_SIZE);
        DrawLine(offsetX, lineY, offsetX + BOARD_PIXEL_WIDTH, lineY, GRAY);
    }

    // Draw blocks (skip animated blocks if swap is active)
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            // Skip blocks being animated
            if (swapAnim->active && y == swapAnim->y &&
                (x == swapAnim->x || x == swapAnim->x + 1)) {
                continue;
            }

            uint16_t cell = board->grid[GRID_INDEX(x, y)];
            BlockType type = BLOCK_TYPE(cell);
            BlockState state = BLOCK_STATE(cell);
            int pixelX = offsetX + (x * BLOCK_SIZE);
            int pixelY = offsetY + (y * BLOCK_SIZE);
            DrawBlockWithState(type, state, pixelX, pixelY);
        }
    }

    // Draw animated blocks
    if (swapAnim->active) {
        int leftGridX = swapAnim->x;
        int rightGridX = swapAnim->x + 1;
        int gridY = swapAnim->y;

        // Get block types (they've already been swapped in the board data)
        // So the "left" position now contains what was on the right, and vice versa
        BlockType leftType = BLOCK_TYPE(board->grid[GRID_INDEX(leftGridX, gridY)]);
        BlockType rightType = BLOCK_TYPE(board->grid[GRID_INDEX(rightGridX, gridY)]);

        // Calculate animation offset
        float animOffset = (1.0f - swapAnim->progress) * BLOCK_SIZE;

        // Left block (was right, moving left): starts from right, ends at left
        int leftPixelX = offsetX + (leftGridX * BLOCK_SIZE) + (int)animOffset;
        int leftPixelY = offsetY + (gridY * BLOCK_SIZE);
        Renderer_DrawBlockAtPixel(leftType, leftPixelX, leftPixelY);

        // Right block (was left, moving right): starts from left, ends at right
        int rightPixelX = offsetX + (rightGridX * BLOCK_SIZE) - (int)animOffset;
        int rightPixelY = offsetY + (gridY * BLOCK_SIZE);
        Renderer_DrawBlockAtPixel(rightType, rightPixelX, rightPixelY);
    }

    // Draw grid coordinates (for debugging)
    for (int x = 0; x < BOARD_WIDTH; x++) {
        int pixelX = offsetX + (x * BLOCK_SIZE) + (BLOCK_SIZE / 2) - 4;
        int pixelY = offsetY + BOARD_PIXEL_HEIGHT + 5;
        DrawText(TextFormat("%d", x), pixelX, pixelY, 10, LIGHTGRAY);
    }
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        int pixelX = offsetX - 15;
        int pixelY = offsetY + (y * BLOCK_SIZE) + (BLOCK_SIZE / 2) - 5;
        DrawText(TextFormat("%d", y), pixelX, pixelY, 10, LIGHTGRAY);
    }

    // Draw board outline
    DrawRectangleLines(offsetX, offsetY, BOARD_PIXEL_WIDTH, BOARD_PIXEL_HEIGHT, WHITE);
}

int Renderer_GetCenteredOffsetX(void)
{
    return (WINDOW_WIDTH - BOARD_PIXEL_WIDTH) / 2;
}

int Renderer_GetCenteredOffsetY(void)
{
    return (WINDOW_HEIGHT - BOARD_PIXEL_HEIGHT) / 2;
}

// Cursor styling constants
static const Color CURSOR_COLOR = ORANGE;
static const Color CURSOR_BORDER_COLOR = { 139, 69, 0, 255 };  // Dark orange/brown
static const int CURSOR_MAIN_THICKNESS = 3;
static const int CURSOR_BORDER_THICKNESS = 1;

void Renderer_DrawCursor(int gridX, int gridY, int offsetX, int offsetY)
{
    int pixelX = offsetX + (gridX * BLOCK_SIZE);
    int pixelY = offsetY + (gridY * BLOCK_SIZE);
    int width = BLOCK_SIZE * 2;
    int height = BLOCK_SIZE;

    // Draw outer dark border for depth
    DrawRectangleLinesEx(
        (Rectangle){ pixelX - CURSOR_BORDER_THICKNESS, pixelY - CURSOR_BORDER_THICKNESS,
                     width + (CURSOR_BORDER_THICKNESS * 2), height + (CURSOR_BORDER_THICKNESS * 2) },
        CURSOR_BORDER_THICKNESS,
        CURSOR_BORDER_COLOR
    );

    // Draw main cursor border (distinct from all block colors)
    DrawRectangleLinesEx(
        (Rectangle){ pixelX, pixelY, width, height },
        CURSOR_MAIN_THICKNESS,
        CURSOR_COLOR
    );

    // Draw inner dark border for depth
    DrawRectangleLinesEx(
        (Rectangle){ pixelX + CURSOR_MAIN_THICKNESS, pixelY + CURSOR_MAIN_THICKNESS,
                     width - (CURSOR_MAIN_THICKNESS * 2), height - (CURSOR_MAIN_THICKNESS * 2) },
        CURSOR_BORDER_THICKNESS,
        CURSOR_BORDER_COLOR
    );
}
