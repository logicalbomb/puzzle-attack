# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Puzzle Attack is a competitive match-3 puzzle game written in C using raylib. Players match 3+ colored blocks horizontally or vertically to clear them and send "garbage blocks" to attack opponents. The game uses a client/server architecture with reliable UDP networking (similar to Quake 3 Arena).

## Tech Stack

- **Language:** C
- **Graphics/Audio:** raylib (v5.0+)
- **Networking:** ENet (recommended) or custom reliable UDP
- **Build System:** Make or CMake
- **Target Platforms:** Windows, macOS, Linux

## Project Structure (Planned)

```
puzzle-attack/
├── src/
│   ├── client/      # Rendering, input, network client, audio
│   ├── server/      # Headless game server, connection management
│   ├── shared/      # Game logic used by both (board, matches, physics)
│   └── main.c       # Entry point
├── include/         # Header files
├── assets/
│   ├── sounds/
│   └── music/
├── build/           # Compiled binaries (gitignored)
└── docs/            # Project documentation
```

## Architecture

- **Server:** Authoritative game state owner. Runs headless, validates player moves, processes match detection, and broadcasts state updates. Prevents cheating.
- **Client:** Handles rendering, input collection, and network communication. Uses client-side prediction for responsiveness with server reconciliation.
- **Shared:** Game logic (board operations, match detection, gravity, attacks) compiled for both client and server.

## Key Game Concepts

- **Board:** 6 columns x 12 rows grid
- **Block Types:** EMPTY, RED, BLUE, GREEN, YELLOW, PURPLE, GARBAGE
- **Block States:** NORMAL, FALLING, MATCHED, LOCKED
- **Attacks:** Combos generate garbage blocks sent to opponent (combo_level - 1) * 2 blocks
- **Win Condition:** Opponent's board fills to top, or higher score at time limit

## Performance Targets

- 60 FPS during gameplay
- Executable size < 100 MB
- Network latency tolerance < 100ms
- 0 runtime crashes

## Development Phases

The project follows a phased approach documented in `docs/puzzle-attack-release-plan.md`:
1. **Phase 0:** Foundation (environment setup, project structure, basic raylib window)
2. **Phase 1:** Core single-player gameplay (board, input, matching, gravity, combos)
3. **Phase 2:** Adversarial features (attacks, dual boards, local multiplayer)
4. **Phase 3:** Networking (client/server separation, matchmaking, prediction)
5. **Phase 4:** Polish (audio, VFX, balance, cross-platform builds)

## Task Reference

Detailed task descriptions are in `docs/puzzle-attack-task-list.md`. Each task has:
- Task ID (e.g., GAME-001)
- Requirements and success criteria
- Suggested file locations and function signatures

When implementing tasks, reference the task ID and follow the specified structure.
