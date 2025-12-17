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

void Renderer_DrawBlock(BlockType type, int gridX, int gridY, int offsetX, int offsetY)
{
    if (type == BLOCK_EMPTY) {
        return;
    }

    int pixelX = offsetX + (gridX * BLOCK_SIZE);
    int pixelY = offsetY + (gridY * BLOCK_SIZE);

    Color color = GetBlockColor(type);

    // Draw filled block with small padding for visual separation
    int padding = 2;
    DrawRectangle(
        pixelX + padding,
        pixelY + padding,
        BLOCK_SIZE - (padding * 2),
        BLOCK_SIZE - (padding * 2),
        color
    );
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
