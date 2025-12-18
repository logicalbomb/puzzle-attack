#include "match_detection.h"
#include <string.h>

// Ceiling division: rounds up a/b
#define CEIL_DIV(a, b) (((a) + (b) - 1) / (b))

// Bitmask array size: ceiling of BOARD_SIZE / 8 (bits packed into bytes)
#define MARKED_ARRAY_SIZE CEIL_DIV(BOARD_SIZE, 8)

// Bitmask helpers
static inline void SetMarked(uint8_t* marked, int index)
{
    marked[index / 8] |= (1 << (index % 8));
}

static inline bool IsMarked(const uint8_t* marked, int index)
{
    return (marked[index / 8] & (1 << (index % 8))) != 0;
}

// Helper to mark a block as matched (preserves type, sets state to MATCHED)
static void MarkAsMatched(GameBoard* board, int x, int y)
{
    uint16_t cell = GameBoard_GetCell(board, x, y);
    BlockType type = BLOCK_TYPE(cell);
    GameBoard_SetCell(board, x, y, MAKE_BLOCK(type, STATE_MATCHED));
}

// Check horizontal matches starting from each position
static int DetectHorizontalMatches(GameBoard* board, uint8_t* marked)
{
    int matchCount = 0;

    for (int y = 0; y < BOARD_HEIGHT; y++) {
        int runStart = 0;
        BlockType runType = BLOCK_TYPE(GameBoard_GetCell(board, 0, y));
        int runLength = 1;

        for (int x = 1; x <= BOARD_WIDTH; x++) {
            BlockType currentType = (x < BOARD_WIDTH)
                ? BLOCK_TYPE(GameBoard_GetCell(board, x, y))
                : BLOCK_EMPTY;

            if (currentType == runType && runType != BLOCK_EMPTY) {
                runLength++;
            } else {
                // End of run - check if it's a match
                if (runLength >= MIN_MATCH_LENGTH && runType != BLOCK_EMPTY) {
                    for (int i = runStart; i < runStart + runLength; i++) {
                        int index = GRID_INDEX(i, y);
                        if (!IsMarked(marked, index)) {
                            SetMarked(marked, index);
                            matchCount++;
                        }
                        MarkAsMatched(board, i, y);
                    }
                }

                // Start new run
                runStart = x;
                runType = currentType;
                runLength = 1;
            }
        }
    }

    return matchCount;
}

// Check vertical matches starting from each position
static int DetectVerticalMatches(GameBoard* board, uint8_t* marked)
{
    int matchCount = 0;

    for (int x = 0; x < BOARD_WIDTH; x++) {
        int runStart = 0;
        BlockType runType = BLOCK_TYPE(GameBoard_GetCell(board, x, 0));
        int runLength = 1;

        for (int y = 1; y <= BOARD_HEIGHT; y++) {
            BlockType currentType = (y < BOARD_HEIGHT)
                ? BLOCK_TYPE(GameBoard_GetCell(board, x, y))
                : BLOCK_EMPTY;

            if (currentType == runType && runType != BLOCK_EMPTY) {
                runLength++;
            } else {
                // End of run - check if it's a match
                if (runLength >= MIN_MATCH_LENGTH && runType != BLOCK_EMPTY) {
                    for (int i = runStart; i < runStart + runLength; i++) {
                        int index = GRID_INDEX(x, i);
                        if (!IsMarked(marked, index)) {
                            SetMarked(marked, index);
                            matchCount++;
                        }
                        MarkAsMatched(board, x, i);
                    }
                }

                // Start new run
                runStart = y;
                runType = currentType;
                runLength = 1;
            }
        }
    }

    return matchCount;
}

int DetectMatches(GameBoard* board)
{
    // Track which blocks have been counted to avoid double-counting
    // L-shaped and T-shaped matches. Uses bitmask: 72 cells / 8 = 9 bytes
    uint8_t marked[MARKED_ARRAY_SIZE];
    memset(marked, 0, sizeof(marked));

    int totalMatched = 0;

    // Detect horizontal matches first
    totalMatched += DetectHorizontalMatches(board, marked);

    // Detect vertical matches (uses same marked array to avoid double-counting)
    totalMatched += DetectVerticalMatches(board, marked);

    return totalMatched;
}

bool HasMatchedBlocks(const GameBoard* board)
{
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (BLOCK_STATE(board->grid[i]) == STATE_MATCHED) {
            return true;
        }
    }
    return false;
}
