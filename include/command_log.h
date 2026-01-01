#ifndef COMMAND_LOG_H
#define COMMAND_LOG_H

#include <stdint.h>
#include <stdbool.h>

// Maximum commands per game session
#define MAX_COMMANDS 4096

// Command types
typedef enum {
    CMD_GAME_START,
    CMD_CURSOR_MOVE,
    CMD_SWAP,
    CMD_MANUAL_RAISE,
    CMD_AUTO_RAISE,
    CMD_MATCH_DETECTED,
    CMD_BLOCKS_CLEARED,
    CMD_GAME_OVER
} CommandType;

// Direction for cursor moves
typedef enum {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} Direction;

// Single command entry
typedef struct {
    uint32_t frame;
    CommandType type;
    union {
        Direction direction;           // CMD_CURSOR_MOVE
        struct { int x, y; } swap;     // CMD_SWAP
        int matchCount;                // CMD_MATCH_DETECTED
        struct { int count, score; } clear;  // CMD_BLOCKS_CLEARED
        int finalScore;                // CMD_GAME_OVER
    } data;
} Command;

// Command log for a game session
typedef struct {
    uint64_t seed;
    uint32_t commandCount;
    Command commands[MAX_COMMANDS];
} CommandLog;

// Initialize command log with seed
void CommandLog_Init(CommandLog* log, uint64_t seed);

// Clear command log (keeps seed)
void CommandLog_Clear(CommandLog* log);

// Record commands
void CommandLog_RecordGameStart(CommandLog* log, uint32_t frame);
void CommandLog_RecordCursorMove(CommandLog* log, uint32_t frame, Direction dir);
void CommandLog_RecordSwap(CommandLog* log, uint32_t frame, int x, int y);
void CommandLog_RecordManualRaise(CommandLog* log, uint32_t frame);
void CommandLog_RecordAutoRaise(CommandLog* log, uint32_t frame);
void CommandLog_RecordMatchDetected(CommandLog* log, uint32_t frame, int count);
void CommandLog_RecordBlocksCleared(CommandLog* log, uint32_t frame, int count, int score);
void CommandLog_RecordGameOver(CommandLog* log, uint32_t frame, int finalScore);

// Dump log to file
// Returns true on success
bool CommandLog_DumpToFile(const CommandLog* log, const char* filename);

#endif // COMMAND_LOG_H
