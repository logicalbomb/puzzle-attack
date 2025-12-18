# Puzzle Attack - Task List (Quick Reference)

## How to Use This with Claude Code

Copy the task description and paste it into Claude Code. Each task is designed to be self-contained and achievable in one session.

**Format:**
```
Task ID: [PHASE-NUMBER]
Description: [What to build]
Context: [Why/how it fits]
Success: [How to verify]
```

---

## PHASE 0: FOUNDATION (Week 1-2)

### ENV-001: Development Environment Setup ✅ COMPLETED
**Description:** Set up C compiler, raylib, and build system for cross-platform compilation

**Context:** This is the foundation for the entire project. We need raylib for graphics/audio and a reliable build system.

**Requirements:**
- Install C compiler (GCC or Clang)
- Install raylib library (v5.0+)
- Create Makefile or CMakeLists.txt
- Support for Windows, macOS, and Linux builds

**Success Criteria:**
- `make` (or `cmake`) successfully compiles a hello-world C program
- raylib headers are found by compiler
- Can generate executable for current platform

**Files to Create:**
- `Makefile` or `CMakeLists.txt`
- `.gitignore` (for build artifacts)

---

### ARCH-001: Project Directory Structure ✅ COMPLETED
**Description:** Create organized folder structure for client/server architecture

**Context:** Separating client, server, and shared code now will make networking easier later.

**Requirements:**
Create this structure:
```
puzzle-attack/
├── src/
│   ├── client/      # Client-specific code
│   ├── server/      # Server-specific code (headless)
│   ├── shared/      # Game logic used by both
│   └── main.c       # Entry point
├── include/         # Header files
├── assets/          # Game resources
│   ├── sounds/
│   └── music/
├── build/           # Compiled binaries (gitignored)
├── docs/            # Documentation
├── Makefile
└── README.md
```

**Success Criteria:**
- All directories exist
- README.md has basic project description
- .gitignore excludes build/ directory

---

### TEST-001: Basic Raylib Window ✅ COMPLETED
**Description:** Create minimal raylib application that opens a window

**Context:** This validates our setup and creates the game loop foundation.

**Requirements:**
- Initialize 800x600 window titled "Puzzle Attack"
- Set target FPS to 60
- Display "Puzzle Attack - Press ESC to quit" text
- Show FPS counter in corner
- Clean shutdown on ESC key

**Success Criteria:**
- Window opens without errors
- FPS shows ~60
- ESC key closes window cleanly
- No memory leaks (check with valgrind if available)

**File to Create:**
- `src/main.c`

**Sample Code Structure:**
```c
#include "raylib.h"

int main(void) {
    InitWindow(800, 600, "Puzzle Attack");
    SetTargetFPS(60);
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Puzzle Attack", 200, 200, 40, BLACK);
        DrawFPS(10, 10);
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
```

---

## PHASE 1: CORE GAMEPLAY (Dec 16 - Jan 1)

### GAME-001: Game Board Data Structure ✅ COMPLETED
**Description:** Define the data structures to represent the game board and blocks

**Context:** This is the core data model. Everything else builds on this.

**Requirements:**
- Define constants: BOARD_WIDTH (6), BOARD_HEIGHT (12)
- Define BlockType enum as bitfield values: EMPTY, RED, BLUE, GREEN, YELLOW, PURPLE
- Define BlockState enum: NORMAL, FALLING, MATCHED, LOCKED
- Grid uses uint16_t where lower byte is BlockType bitfield, upper byte is BlockState
- Create GameBoard struct with: uint16_t grid[BOARD_HEIGHT][BOARD_WIDTH]
- Provide helper macros/functions to extract type and state from cell value

**Success Criteria:**
- Structures compile without errors
- Can create a GameBoard instance
- Can access individual blocks: board.grid[y][x]
- Can extract type and state from a cell value

**Files to Create:**
- `include/game_board.h`
- `src/shared/game_board.c`

**Example:**
```c
#include <stdint.h>

// Block types (lower byte - bitfield)
typedef enum {
    BLOCK_EMPTY  = 0x00,
    BLOCK_RED    = 0x01,
    BLOCK_BLUE   = 0x02,
    BLOCK_GREEN  = 0x04,
    BLOCK_YELLOW = 0x08,
    BLOCK_PURPLE = 0x10
} BlockType;

// Block states (upper byte)
typedef enum {
    STATE_NORMAL  = 0x00,
    STATE_FALLING = 0x01,
    STATE_MATCHED = 0x02,
    STATE_LOCKED  = 0x04
} BlockState;

// Helper macros
#define BLOCK_TYPE(cell)  ((BlockType)((cell) & 0xFF))
#define BLOCK_STATE(cell) ((BlockState)(((cell) >> 8) & 0xFF))
#define MAKE_BLOCK(type, state) ((uint16_t)(((state) << 8) | (type)))

// Grid indexing (1D array for simpler serialization)
#define BOARD_SIZE (BOARD_WIDTH * BOARD_HEIGHT)
#define GRID_INDEX(x, y) ((y) * BOARD_WIDTH + (x))

typedef struct {
    uint16_t grid[BOARD_SIZE];
    int score;
} GameBoard;
```

---

### GAME-002: Board Rendering ✅ COMPLETED
**Description:** Draw the game board and blocks on screen

**Context:** We need to see the board to develop the game.

**Requirements:**
- Define block size (60x60 pixels)
- Calculate board position (centered in window)
- Draw grid outline
- Draw each block as colored rectangle
- Use raylib colors: RED, BLUE, GREEN, YELLOW, PURPLE
- Draw grid coordinates (for debugging)

**Success Criteria:**
- Board displays in center of window
- All blocks visible with distinct colors
- Grid lines show boundaries
- Can see 6x12 grid clearly

**Files to Modify:**
- `src/client/renderer.c` (create)
- `include/renderer.h` (create)
- `src/main.c` (update to call render function)

**Function Signature:**
```c
void RenderBoard(GameBoard* board, int offsetX, int offsetY);
```

---

### GAME-003: Board Initialization ✅ COMPLETED
**Description:** Create function to populate board with random blocks

**Context:** Need starting state for the game.

**Requirements:**
- Fill entire board with random colored blocks
- Avoid creating initial matches (3+ in a row)
- Use GetRandomValue() for randomization
- Set all blocks to NORMAL state initially

**Success Criteria:**
- Board fills completely
- No matches exist at start
- Different games have different layouts
- Visual verification: run game multiple times, board looks different

**Files to Create/Modify:**
- `src/shared/board_init.c`
- Add `InitializeBoard(GameBoard* board)` function

**Algorithm Hint:**
1. For each cell, generate random block type
2. Check if it creates a match with previous blocks
3. If match, reroll until no match
4. Max retries: 10 (then accept any block)

---

### INPUT-001: Cursor Selection System ✅ COMPLETED
**Description:** Implement player cursor to select blocks

**Context:** Players need to choose which blocks to swap.

**Requirements:**
- Cursor highlights two adjacent horizontal blocks (for swapping)
- Arrow keys move cursor (up/down/left/right)
- Cursor wraps around edges (optional)
- Cursor cannot go out of bounds (x: 0 to BOARD_WIDTH-2)
- Visual highlight: draw border around selected block pair
- Cursor color must be distinct from block colors (use orange, not yellow)
- Dark outline around cursor for depth and visibility

**Success Criteria:**
- Arrow keys move cursor
- Two horizontal blocks have visible highlight (orange border with dark outline for depth)
- Cursor color is distinct from all block colors (uses orange, not yellow)
- Cursor stays within valid range (x: 0-4, y: 0-11)
- Smooth, responsive controls

**Files to Create:**
- `src/client/input.c`
- `include/input.h`

**Data Structure:**
```c
typedef struct {
    int x, y;  // Current cursor position (left block of the pair)
} Cursor;
// Cursor covers blocks at (x, y) and (x+1, y)
```

---

### INPUT-002: Block Swapping ✅ COMPLETED
**Description:** Allow player to swap the two blocks under the cursor

**Context:** Core mechanic of match-3 games. The cursor covers two horizontal blocks.

**Requirements:**
- Press SPACE to swap the two blocks under the cursor
- Cursor position determines which blocks are swapped: (x, y) and (x+1, y)
- Swap always succeeds (no swap-back if no match is created)
- Animate swap (blocks slide to new positions)
- Only one swap can happen at a time

**Success Criteria:**
- Blocks swap smoothly when SPACE is pressed
- Swap animation takes ~0.2 seconds
- Board state updates correctly
- Swaps persist regardless of whether a match occurs

**Files to Modify:**
- `src/client/input.c`
- `src/shared/game_logic.c` (create)

**Function:**
```c
void SwapBlocks(GameBoard* board, int x, int y);
// Swaps blocks at (x, y) and (x+1, y)
```

---

### MATCH-001: Match Detection ✅ COMPLETED
**Description:** Detect when 3+ blocks of same color align

**Context:** Essential game mechanic - clearing matches.

**Requirements:**
- Check horizontal matches (3+ in a row)
- Check vertical matches (3+ in a column)
- Mark matched blocks (set state to MATCHED)
- Support matches of 4, 5, or more blocks
- Return count of total blocks matched

**Success Criteria:**
- Correctly identifies 3-block matches
- Correctly identifies 4+ block matches
- Handles multiple simultaneous matches
- Handles L-shaped and T-shaped matches (blocks matched twice only count once)

**Files:**
- `src/shared/match_detection.c`
- `include/match_detection.h`

**Function:**
```c
int DetectMatches(GameBoard* board);
// Returns: number of blocks matched
```

**Algorithm:**
- Scan each row for consecutive same-color blocks
- Scan each column for consecutive same-color blocks
- Mark all matched blocks with STATE_MATCHED
- Use bitmask array (uint8_t[CEIL_DIV(BOARD_SIZE, 8)]) to track counted blocks and avoid double-counting L/T shapes
- Return total count

---

### MATCH-002: Block Clearing
**Description:** Remove matched blocks from the board

**Context:** After detecting matches, we need to clear them.

**Requirements:**
- Find all blocks with MATCHED state
- Set their type to BLOCK_EMPTY
- Add to score (3-match = 30 pts, 4-match = 50 pts, 5-match = 100 pts)
- Optional: flash animation before clearing
- Play match sound effect (if audio implemented)

**Success Criteria:**
- Matched blocks disappear
- Score increases correctly
- Visual feedback shows which blocks were cleared
- Board state is consistent

**Files to Modify:**
- `src/shared/game_logic.c`

**Function:**
```c
void ClearMatches(GameBoard* board);
```

---

### PHYS-001: Gravity System
**Description:** Make blocks fall down to fill empty spaces

**Context:** After clearing, blocks above should fall.

**Requirements:**
- Scan columns from bottom to top
- For each empty space, move block above it down
- Continue until no empty spaces have blocks above
- Animate falling (blocks smoothly drop)
- Set falling blocks to FALLING state during animation

**Success Criteria:**
- All blocks settle to bottom
- No floating blocks
- Smooth animation (blocks fall at constant speed)
- Multiple blocks can fall simultaneously

**Files:**
- `src/shared/physics.c`
- `include/physics.h`

**Function:**
```c
bool ApplyGravity(GameBoard* board);
// Returns: true if any blocks moved
```

---

### PHYS-002: Board Refill
**Description:** Generate new blocks at the top after gravity settles

**Context:** After blocks fall, top of board has empty spaces.

**Requirements:**
- For each empty cell in top row, generate random block
- New blocks should "spawn" above board and fall in
- Animate entry
- Use same random generation as initialization

**Success Criteria:**
- Board is always full (no persistent empty spaces)
- New blocks match color palette
- Smooth spawn animation
- Randomized block types

**Files to Modify:**
- `src/shared/physics.c`

**Function:**
```c
void RefillBoard(GameBoard* board);
```

---

### GAME-004: Cascade/Combo System
**Description:** Detect and handle chain reactions

**Context:** When new blocks fall, they might create new matches.

**Requirements:**
- After gravity settles, check for new matches
- If matches found, clear them and apply gravity again
- Count cascades as "combo"
- Increase score multiplier for each combo level
- Stop when no new matches appear

**Success Criteria:**
- Chain reactions work automatically
- Combo counter increases
- Score multiplier applies (2x for combo 2, 3x for combo 3, etc.)
- Game doesn't get stuck in infinite loop

**Files to Modify:**
- `src/shared/game_logic.c`

**Game Loop Update:**
```c
while (matchesExist) {
    DetectMatches(board);
    ClearMatches(board);
    ApplyGravity(board);
    RefillBoard(board);
    comboCount++;
}
```

---

### UI-001: Score Display
**Description:** Show score and combo counter on screen

**Context:** Players need feedback on performance.

**Requirements:**
- Display current score (top-left)
- Display combo counter (top-center)
- Display high score (if implemented)
- Update in real-time
- Combo resets to 0 when no matches occur

**Success Criteria:**
- Score visible and updates correctly
- Combo displays during chain reactions
- Text is readable (good contrast)

**Files:**
- `src/client/ui.c`
- `include/ui.h`

---

### UI-002: Game States
**Description:** Implement state machine for game flow

**Context:** Need menu, gameplay, pause, and game over states.

**Requirements:**
- Define GameState enum: MENU, PLAYING, PAUSED, GAME_OVER
- Implement state transitions
- Menu: display title, "Press ENTER to start"
- Pause: press P to pause/unpause
- Game Over: show final score, "Press R to restart"

**Success Criteria:**
- Clean transitions between states
- Input works correctly in each state
- Can restart game from game over

**Files to Create:**
- `src/shared/game_state.c`
- `include/game_state.h`

---

### GAME-005: Game Over Condition
**Description:** End game when time/moves run out or board fills

**Context:** Games need an end condition.

**Requirements:**
Choose one:
- **Option A:** Time limit (2 minutes)
- **Option B:** Move limit (20 swaps)
- **Option C:** Board fills to top (no valid swaps)

**Success Criteria:**
- Game ends at appropriate time
- Transition to GAME_OVER state
- Final score displayed
- Can restart game

**Files to Modify:**
- `src/shared/game_logic.c`

---

## PHASE 2: ADVERSARIAL FEATURES (Jan 1 - Feb 1)

### ATTACK-001: Attack Block Design
**Description:** Define garbage block mechanics

**Context:** Players attack each other by sending garbage blocks.

**Requirements:**
- Add BLOCK_GARBAGE to BlockType enum
- Garbage blocks don't match
- Garbage blocks clear when adjacent colored block matches
- Define attack strength formula: combo_level * 2 = garbage blocks sent

**Success Criteria:**
- Garbage blocks render differently (gray color)
- Cannot be swapped like normal blocks
- Clear when neighboring block matches

---

### ATTACK-002: Attack Generation
**Description:** Generate attacks from combos

**Context:** Rewards players for creating cascades.

**Requirements:**
- Track combo level during cascade
- When cascade ends, calculate attack size
- Formula: max(combo_level - 1, 0) * 2 blocks
- Queue attack for opponent
- Visual feedback: show "ATTACK!" text with size

**Success Criteria:**
- 3-combo sends 4 garbage blocks
- 4-combo sends 6 garbage blocks
- Attack queues properly

**Files:**
- `src/shared/attack_system.c`

---

### ATTACK-003: Attack Delivery
**Description:** Add garbage blocks to opponent's board

**Context:** Execute the attack on opponent.

**Requirements:**
- Insert garbage blocks from bottom
- Shift existing blocks up
- Random column placement or spread evenly
- Animate garbage blocks rising
- Check if board overflow (top fills) → game over

**Success Criteria:**
- Garbage appears on correct board
- Existing blocks shift up
- Game over if board overflows

---

### UI-003: Dual Board Display
**Description:** Show two boards side-by-side

**Context:** Need to see both players' boards.

**Requirements:**
- Player 1 board on left (x=100)
- Player 2 board on right (x=500)
- Label each board: "PLAYER 1" and "PLAYER 2"
- Share same render function, different offsets
- Increase window width to 1000+

**Success Criteria:**
- Both boards visible
- No overlap
- Clear player labels

**Files to Modify:**
- `src/client/renderer.c`
- `src/main.c` (update window size)

---

### GAME-006: Independent Board States
**Description:** Each player has their own game board

**Context:** Two separate games running simultaneously.

**Requirements:**
- Create array: GameBoard boards[2]
- Each board processes matches independently
- Attacks flow from boards[0] → boards[1] and vice versa
- Each board has own score, combo counter

**Success Criteria:**
- Both boards update independently
- Swapping on one board doesn't affect the other
- Attacks successfully sent between boards

---

### INPUT-003: Split Input Handling
**Description:** Support two players on same keyboard

**Context:** Local multiplayer controls.

**Requirements:**
**Player 1:**
- Move: W/A/S/D
- Select/Swap: E or SPACE

**Player 2:**
- Move: Arrow Keys
- Select/Swap: ENTER or Right SHIFT

**Success Criteria:**
- Both players can control simultaneously
- No input conflicts
- Responsive controls for both

**Files to Modify:**
- `src/client/input.c`

---

### GAME-007: Win/Loss Conditions
**Description:** Determine winner in versus mode

**Context:** Need competitive end conditions.

**Requirements:**
- Game ends when:
  - One player's board fills to top (they lose)
  - Time limit reached (higher score wins)
  - (Optional) First to X score wins
- Display winner
- Transition to game over state

**Success Criteria:**
- Correct winner determined
- Clear "PLAYER X WINS!" message
- Both boards stop updating

---

### GAME-008: Versus Mode UI
**Description:** Add competitive UI elements

**Context:** Show competitive information.

**Requirements:**
- Attack queue indicator (pending garbage count)
- "Danger zone" highlight (top 2 rows turn red)
- Combo display for each player
- Timer (if using time limit)

**Success Criteria:**
- Players can see their attack queue
- Danger zone clearly visible when board getting full
- All info readable during intense gameplay

---

## PHASE 3: NETWORKING (Feb 1 - March 1)

### NET-001: Network Library Research
**Description:** Choose and integrate networking library

**Context:** Need reliable UDP for game networking.

**Options:**
- ENet (recommended - Quake-like protocol)
- Custom implementation with sockets

**Requirements:**
- Research ENet library
- Add to build system
- Test basic send/receive

**Success Criteria:**
- Can send packet from client to server
- Can receive packet response
- Library compiles on all platforms

---

### NET-002: Network Protocol Design
**Description:** Define message format for client-server communication

**Context:** Need standardized packet structure.

**Requirements:**
Define packet types:
```c
enum PacketType {
    PKT_CONNECT,
    PKT_DISCONNECT,
    PKT_INPUT,        // Player swap command
    PKT_STATE_UPDATE, // Board state sync
    PKT_MATCH_EVENT,  // Match/attack occurred
    PKT_GAME_OVER
};
```

**Success Criteria:**
- Clear packet structure documented
- Efficient serialization (binary format)
- Each packet type has size limit

**Files:**
- `include/network_protocol.h`

---

### ARCH-002: Separate Client/Server
**Description:** Refactor codebase for client-server architecture

**Context:** Game logic must run on server, client only renders.

**Requirements:**
- Move game logic to `src/shared/`
- Create `src/server/server_main.c` (headless)
- Update `src/client/client_main.c` (rendering + network)
- Game logic functions work without rendering

**Success Criteria:**
- Server compiles without raylib dependency
- Client compiles with raylib
- Shared code compiles for both

---

### SERVER-001: Headless Server
**Description:** Create server that runs without graphics

**Context:** Server must run in background, process game state.

**Requirements:**
- Accept connections on port 7777
- Support 2 clients (1v1 match)
- Process input packets
- Update game state at 60 ticks/second
- Send state updates to clients

**Success Criteria:**
- Server runs without crashing
- Can connect with telnet/nc for testing
- Logs connections/disconnections

**Files:**
- `src/server/server_main.c`
- `src/server/connection_manager.c`

---

### SERVER-002: Authoritative Game State
**Description:** Server owns true game state, validates moves

**Context:** Prevent cheating - server must validate everything.

**Requirements:**
- Server maintains GameBoard for each player
- Client sends swap requests
- Server validates swap is legal
- Server processes match detection
- Server broadcasts state updates

**Success Criteria:**
- Invalid swaps rejected (logged)
- Matches detected server-side
- Clients receive consistent state

**Files:**
- `src/server/game_server.c`

---

### SERVER-003: Matchmaking/Lobby
**Description:** Simple lobby system for player matching

**Context:** Players need to find each other.

**Requirements:**
- Player queue (waiting for opponent)
- When 2 players ready, create game room
- Assign Player 1 and Player 2
- Start game countdown (3... 2... 1... GO!)

**Success Criteria:**
- Two clients connect and get matched
- Game starts automatically
- Players assigned to correct boards

---

### CLIENT-001: Network Client
**Description:** Connect client to server and send/receive packets

**Context:** Client needs to communicate with server.

**Requirements:**
- Connect to server IP:port
- Send player input (swap commands)
- Receive state updates
- Handle disconnection gracefully

**Success Criteria:**
- Client connects to localhost server
- Input sent successfully
- State updates received and applied

**Files:**
- `src/client/network_client.c`

---

### CLIENT-002: Client-Side Prediction
**Description:** Predict moves locally for responsiveness

**Context:** Players shouldn't wait for server round-trip.

**Requirements:**
- When player swaps, predict immediately
- Render predicted state
- When server response arrives, reconcile
- If prediction wrong, snap to server state

**Success Criteria:**
- Swaps feel instant
- Mispredictions are rare
- Corrections are smooth (not jarring)

---

### CLIENT-003: Network UI
**Description:** Add networking status to UI

**Context:** Players need to know connection state.

**Requirements:**
- Connection status indicator (Connected/Disconnected)
- Ping display (in ms)
- Lobby screen: "Waiting for opponent..."
- "Connection lost" message

**Success Criteria:**
- Can see when connected
- Ping updates every second
- Clear feedback during lobby wait

---

### NET-003: Network Testing
**Description:** Test under adverse conditions

**Context:** Ensure game works with latency/packet loss.

**Requirements:**
- Test with simulated latency (50ms, 100ms, 200ms)
- Test with packet loss (1%, 5%)
- Test rapid disconnect/reconnect
- Log all anomalies

**Success Criteria:**
- Playable under 100ms latency
- Recovers from brief disconnects
- No crashes from malformed packets

---

### NET-004: Bandwidth Optimization
**Description:** Reduce network traffic

**Context:** Keep bandwidth low for smooth play.

**Requirements:**
- Delta compression (only send changes)
- Reduce update frequency where possible (30 Hz instead of 60 Hz)
- Measure bytes/second usage
- Target: <50 KB/s per client

**Success Criteria:**
- Bandwidth logged and reasonable
- Game still feels smooth
- No visual stuttering

---

## PHASE 4: POLISH (March 1 - April 1)

### AUDIO-001: Sound Effects
**Description:** Add audio feedback for actions

**Context:** Sound makes game feel juicy.

**Requirements:**
- Block swap sound (soft click)
- Match sound (ding, pitched by match size)
- Attack sent (whoosh)
- Attack received (thud)
- Combo sound (ascending chime)
- Game over (dramatic sound)

**Success Criteria:**
- All sounds play at correct times
- Sounds don't overlap badly
- Volume is balanced

**Files:**
- `assets/sounds/` (add .wav or .ogg files)
- `src/client/audio.c`

---

### AUDIO-002: Background Music
**Description:** Add looping music tracks

**Context:** Music enhances atmosphere.

**Requirements:**
- Menu music (calm, inviting)
- Gameplay music (upbeat, energetic)
- Victory music (triumphant)
- Defeat music (sad trombone)
- Music loops seamlessly

**Success Criteria:**
- Music plays continuously
- Smooth transitions between tracks
- Can be muted in settings

---

### AUDIO-003: Audio Settings
**Description:** Volume controls and mute

**Context:** Players need audio control.

**Requirements:**
- Master volume slider
- SFX volume slider
- Music volume slider
- Mute toggle
- Settings persist between sessions

**Success Criteria:**
- Settings work immediately
- Saved to config file
- Restored on game launch

---

### VFX-001: Particle Effects
**Description:** Add visual particles for impact

**Context:** Particles add excitement.

**Requirements:**
- Block break particles (colored squares fly out)
- Combo sparkles (shiny particles)
- Attack sending effect (blocks fly toward opponent)
- Victory confetti

**Success Criteria:**
- Particles render smoothly
- Don't cause lag
- Look satisfying

---

### VFX-002: Animation Polish
**Description:** Improve all animations

**Context:** Make movements feel good.

**Requirements:**
- Ease-in/ease-out for swaps
- Squash/stretch on block lands
- Screen shake on big attacks
- Smooth interpolation everywhere

**Success Criteria:**
- Animations feel smooth
- No jarring transitions
- Game feels "juicy"

---

### VFX-003: Visual Feedback
**Description:** Add feedback for game events

**Context:** Players need clear signals.

**Requirements:**
- Flash screen red when receiving attack
- Glow effect on possible matches (optional)
- Victory/defeat screen with animation
- Pulse effect on combo counter

**Success Criteria:**
- Events are immediately recognizable
- Effects not overwhelming
- Clear distinction between events

---

### GAME-009: Balance Tuning
**Description:** Adjust game feel and difficulty

**Context:** Make game fun and fair.

**Requirements:**
- Test attack strength (is 2x per combo too much?)
- Test match scoring (are combos rewarding enough?)
- Test game length (target 2-5 minute matches)
- Adjust based on playtesting

**Success Criteria:**
- Games last 2-5 minutes
- Comebacks are possible
- Skill matters more than luck

---

### UX-001: User Experience Polish
**Description:** Improve menus and onboarding

**Context:** Make game accessible.

**Requirements:**
- Tutorial/How to Play screen
- Better menu navigation (keyboard + mouse)
- Settings menu (audio, controls, graphics)
- Credits screen

**Success Criteria:**
- New players understand controls
- Settings are easy to find
- Menu navigation is smooth

---

### BUILD-001: Windows Build
**Description:** Create Windows executable

**Context:** Most players use Windows.

**Requirements:**
- Compile for Windows (use MinGW or MSVC)
- Test executable size (<100MB)
- Test on Windows 10/11
- Include all DLLs if needed

**Success Criteria:**
- .exe runs without installation
- No missing dependencies
- Meets size requirement

---

### BUILD-002: macOS Build
**Description:** Create macOS executable

**Context:** Support Mac users.

**Requirements:**
- Compile for macOS (clang)
- Create .app bundle
- Test on macOS (Intel and Apple Silicon if possible)
- Code sign if distributing widely

**Success Criteria:**
- .app launches without errors
- Works on multiple macOS versions
- Meets size requirement

---

### BUILD-003: Linux Build
**Description:** Create Linux executable

**Context:** Support Linux users.

**Requirements:**
- Compile for Linux (GCC)
- Test on Ubuntu 22.04 LTS
- Create AppImage or .deb (optional)
- Include setup script

**Success Criteria:**
- Binary runs on Ubuntu
- Dependencies clearly documented
- Meets size requirement

---

### BUILD-004: Distribution Package
**Description:** Create final distribution

**Context:** Make it easy to share.

**Requirements:**
- Zip file with game + assets
- README with controls and requirements
- Installation instructions
- Version number

**Success Criteria:**
- One-click extraction and play
- Clear documentation
- Professional presentation

---

## PHASE 5: NICE-TO-HAVES (April+)

### DISCORD-001: Discord Rich Presence
Show game status in Discord.

### STEAM-001: Steam Integration
Explore Steamworks SDK features.

### MOBILE-001: Mobile Port
Design touch controls and build for Android/iOS.

### DECK-001: Steam Deck Optimization
Add controller support and optimize for handheld.

---

## Tips for Using with Claude Code

1. **Copy one task at a time**
2. **Provide current code context** - share relevant files
3. **Ask for explanation** if task is unclear
4. **Test immediately** after each task
5. **Iterate if needed** - don't hesitate to refine

**Example:**
```
I'm working on Puzzle Attack and ready for task GAME-001.

Current state: I have basic raylib window working (TEST-001 complete).

Please implement the game board data structure as described in GAME-001.
```

Good luck! 🎮
