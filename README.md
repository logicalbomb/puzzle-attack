# Puzzle Attack

A competitive match-3 puzzle game where players match colored blocks to attack opponents with garbage blocks.

## Building

**Requirements:**
- C compiler (GCC or Clang)
- raylib 5.0+
- pkg-config (for Unix systems)

**macOS/Linux:**
```bash
# Install raylib (macOS)
brew install raylib

# Build
make

# Run
make run
```

**Windows (MinGW):**
```bash
# Set RAYLIB_PATH to your raylib installation
make RAYLIB_PATH=C:/raylib/raylib
```

## Controls

- Arrow keys: Move cursor
- Space: Select/swap blocks
- ESC: Quit

## Project Structure

```
puzzle-attack/
├── src/
│   ├── client/      # Rendering, input, audio
│   ├── server/      # Headless game server
│   ├── shared/      # Game logic (board, matches, physics)
│   └── main.c       # Entry point
├── include/         # Header files
├── assets/          # Sounds and music
├── build/           # Compiled binaries
└── docs/            # Documentation
```

## License

Personal project for learning purposes.
