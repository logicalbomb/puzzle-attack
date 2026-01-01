#include "command_log.h"
#include <stdio.h>

// Direction names for output
static const char* DIRECTION_NAMES[] = {
    "UP", "DOWN", "LEFT", "RIGHT"
};

// Add command to log (internal helper)
static bool AddCommand(CommandLog* log, Command cmd)
{
    if (log->commandCount >= MAX_COMMANDS) {
        return false;
    }
    log->commands[log->commandCount++] = cmd;
    return true;
}

void CommandLog_Init(CommandLog* log, uint64_t seed)
{
    log->seed = seed;
    log->commandCount = 0;
}

void CommandLog_Clear(CommandLog* log)
{
    log->commandCount = 0;
}

void CommandLog_RecordGameStart(CommandLog* log, uint32_t frame)
{
    Command cmd = { .frame = frame, .type = CMD_GAME_START };
    AddCommand(log, cmd);
}

void CommandLog_RecordCursorMove(CommandLog* log, uint32_t frame, Direction dir)
{
    Command cmd = {
        .frame = frame,
        .type = CMD_CURSOR_MOVE,
        .data.direction = dir
    };
    AddCommand(log, cmd);
}

void CommandLog_RecordSwap(CommandLog* log, uint32_t frame, int x, int y)
{
    Command cmd = {
        .frame = frame,
        .type = CMD_SWAP,
        .data.swap = { x, y }
    };
    AddCommand(log, cmd);
}

void CommandLog_RecordManualRaise(CommandLog* log, uint32_t frame)
{
    Command cmd = { .frame = frame, .type = CMD_MANUAL_RAISE };
    AddCommand(log, cmd);
}

void CommandLog_RecordAutoRaise(CommandLog* log, uint32_t frame)
{
    Command cmd = { .frame = frame, .type = CMD_AUTO_RAISE };
    AddCommand(log, cmd);
}

void CommandLog_RecordMatchDetected(CommandLog* log, uint32_t frame, int count)
{
    Command cmd = {
        .frame = frame,
        .type = CMD_MATCH_DETECTED,
        .data.matchCount = count
    };
    AddCommand(log, cmd);
}

void CommandLog_RecordBlocksCleared(CommandLog* log, uint32_t frame, int count, int score)
{
    Command cmd = {
        .frame = frame,
        .type = CMD_BLOCKS_CLEARED,
        .data.clear = { count, score }
    };
    AddCommand(log, cmd);
}

void CommandLog_RecordGameOver(CommandLog* log, uint32_t frame, int finalScore)
{
    Command cmd = {
        .frame = frame,
        .type = CMD_GAME_OVER,
        .data.finalScore = finalScore
    };
    AddCommand(log, cmd);
}

bool CommandLog_DumpToFile(const CommandLog* log, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (!file) {
        return false;
    }

    fprintf(file, "SEED: %llu\n", (unsigned long long)log->seed);

    for (uint32_t i = 0; i < log->commandCount; i++) {
        const Command* cmd = &log->commands[i];

        switch (cmd->type) {
            case CMD_GAME_START:
                fprintf(file, "FRAME %u: GAME_START\n", cmd->frame);
                break;
            case CMD_CURSOR_MOVE:
                fprintf(file, "FRAME %u: CURSOR_MOVE %s\n",
                        cmd->frame, DIRECTION_NAMES[cmd->data.direction]);
                break;
            case CMD_SWAP:
                fprintf(file, "FRAME %u: SWAP %d %d\n",
                        cmd->frame, cmd->data.swap.x, cmd->data.swap.y);
                break;
            case CMD_MANUAL_RAISE:
                fprintf(file, "FRAME %u: MANUAL_RAISE\n", cmd->frame);
                break;
            case CMD_AUTO_RAISE:
                fprintf(file, "FRAME %u: AUTO_RAISE\n", cmd->frame);
                break;
            case CMD_MATCH_DETECTED:
                fprintf(file, "FRAME %u: MATCH_DETECTED %d\n",
                        cmd->frame, cmd->data.matchCount);
                break;
            case CMD_BLOCKS_CLEARED:
                fprintf(file, "FRAME %u: BLOCKS_CLEARED %d %d\n",
                        cmd->frame, cmd->data.clear.count, cmd->data.clear.score);
                break;
            case CMD_GAME_OVER:
                fprintf(file, "FRAME %u: GAME_OVER %d\n",
                        cmd->frame, cmd->data.finalScore);
                break;
        }
    }

    fclose(file);
    return true;
}
