# Puzzle Attack - Release Plan & Task Breakdown

## Overview
This release plan breaks down your Puzzle Attack project into 4 major phases aligned with your milestones. Each phase builds on the previous one, allowing you to validate functionality before adding complexity.

**Timeline:** December 2025 → April 2026 (4 months)
**Development Time:** 10-20 hours/week

---

## Phase 0: Foundation & Setup (Week 1-2)
**Goal:** Get development environment ready and basic project structure in place
**Deliverable:** Compiled "Hello World" with raylib rendering a window

### Tasks
- [x] **ENV-001**: Set up development environment (C compiler, raylib, build system)
  - Install GCC/Clang for your platform
  - Install raylib library
  - Set up Makefile or CMake build system
  - Verify cross-compilation capability (if targeting multiple platforms from start)
  
- [x] **ARCH-001**: Create project directory structure
  ```
  puzzle-attack/
  ├── src/
  │   ├── client/
  │   ├── server/
  │   ├── shared/
  │   └── main.c
  ├── include/
  ├── assets/
  │   ├── sounds/
  │   └── music/
  ├── build/
  └── Makefile
  ```

- [x] **TEST-001**: Create basic window with raylib
  - Initialize window (800x600)
  - Set target FPS to 60
  - Basic game loop (BeginDrawing/EndDrawing)
  - Draw text "Puzzle Attack" on screen

**Success Criteria:** Window opens, shows title, closes cleanly. FPS counter visible.

---

## Phase 1: Core Gameplay - Single Player (Dec 16 → Jan 1, 2026)
**Goal:** Build a playable match-3 game for one player
**Deliverable:** Functional single-player puzzle game

### Milestone 1.1: Game Board Rendering (Week 1-2)
- [x] **GAME-001**: Define game board data structure
  - Grid size (e.g., 6x12 board)
  - Block types (colors: red, blue, green, yellow, purple)
  - Block states (normal, falling, matched, empty)

- [x] **GAME-002**: Implement board rendering
  - Draw grid with borders
  - Render blocks as colored squares/sprites
  - Display grid coordinates (for debugging)

- [x] **GAME-003**: Create board initialization
  - Populate board with random blocks
  - Ensure no initial matches exist

### Milestone 1.2: Input & Block Movement (Week 3)
- [x] **INPUT-001**: Implement cursor/selection system
  - Cursor covers two horizontal blocks for swapping
  - Keyboard input (arrow keys to move)
  - Orange cursor with dark border for visibility

- [x] **INPUT-002**: Implement block swapping
  - SPACE key swaps the two blocks under cursor
  - Swap animation (smooth transition)
  - Swaps persist (no swap-back if no match)

### Milestone 1.3: Match Detection & Clearing (Week 4)
- [x] **MATCH-001**: Implement match detection algorithm
  - Horizontal match detection (3+ same color)
  - Vertical match detection (3+ same color)
  - Handle multiple simultaneous matches

- [x] **MATCH-002**: Implement block clearing
  - Remove matched blocks from board
  - Visual feedback for matches (flash/animation)
  - Score calculation based on match size

### Milestone 1.4: Gravity & Refill (Week 5)
- [x] **PHYS-001**: Implement gravity system
  - Blocks fall to fill empty spaces
  - Falling animation
  - Multiple blocks can fall simultaneously

- [ ] **PHYS-002**: Implement board refill
  - Generate new blocks at top
  - Ensure new blocks don't create unintentional matches
  - Smooth entry animation

- [ ] **GAME-004**: Implement cascade/combo system
  - Detect new matches after gravity settles
  - Chain reactions count as combos
  - Combo multiplier for scoring

### Milestone 1.5: Game State & UI (Week 6)
- [ ] **UI-001**: Implement score display
  - Current score
  - Combo counter
  - Optional: high score tracking

- [ ] **UI-002**: Implement game states
  - Menu state (Start/Quit)
  - Playing state
  - Game over state
  - Pause state

- [ ] **GAME-005**: Add basic game over condition
  - Time limit, or
  - Move limit, or
  - Board full condition

**Phase 1 Success Criteria:**
- ✓ Can play a complete game from start to game over
- ✓ Matches detect correctly (3+ blocks)
- ✓ Gravity and refill work without bugs
- ✓ Runs at 60 FPS
- ✓ No crashes during normal play

---

## Phase 2: Adversarial Features (Jan 1 → Feb 1, 2026)
**Goal:** Add attack/defense mechanics for competitive play
**Deliverable:** Two-player game on same machine

### Milestone 2.1: Attack Block System (Week 1-2)
- [ ] **ATTACK-001**: Design attack block mechanics
  - Define "garbage blocks" (gray, unbreakable initially)
  - How attacks are triggered (combo size → attack strength)
  - How garbage blocks clear (matching adjacent colored blocks)

- [ ] **ATTACK-002**: Implement attack generation
  - Generate attacks from combos/large matches
  - Queue attacks to opponent
  - Visual indicator of pending attack

- [ ] **ATTACK-003**: Implement attack delivery
  - Add garbage blocks to opponent's board
  - Animation for incoming attacks
  - Attack placement logic (bottom-up, random columns, etc.)

### Milestone 2.2: Dual Board Display (Week 2-3)
- [ ] **UI-003**: Refactor rendering for two boards
  - Side-by-side board layout
  - Player 1 board (left) and Player 2 board (right)
  - Shared UI elements (timer, attack queue indicators)

- [ ] **GAME-006**: Implement independent board states
  - Each board has its own grid
  - Each board processes matches independently
  - Attacks flow from one board to another

### Milestone 2.3: Local Multiplayer (Week 3-4)
- [ ] **INPUT-003**: Implement split input handling
  - Player 1: WASD + Space/E for selection
  - Player 2: Arrow keys + Enter/Shift for selection
  - OR: Two mouse cursors with keyboard modifiers

- [ ] **GAME-007**: Implement win/loss conditions
  - Board fills to top → player loses
  - Time limit → higher score wins
  - Best of N rounds

- [ ] **GAME-008**: Add versus mode UI
  - Player names/indicators
  - Health bars or "danger meter"
  - Attack warning indicators

**Phase 2 Success Criteria:**
- ✓ Two players can play against each other on same device
- ✓ Attacks successfully impact opponent's board
- ✓ Win/loss conditions work correctly
- ✓ Both boards run smoothly at 60 FPS
- ✓ Clear visual distinction between players

---

## Phase 3: Networking (Feb 1 → March 1, 2026)
**Goal:** Enable networked multiplayer
**Deliverable:** Players can compete from different computers

### Milestone 3.1: Network Architecture (Week 1)
- [ ] **NET-001**: Research reliable UDP implementation
  - Review ENet library (or build custom)
  - Design packet structure
  - Plan client-server protocol

- [ ] **NET-002**: Define network messages
  - Player input (block swap, selection)
  - Game state sync (board state, score)
  - Game events (match, attack, game over)
  - Heartbeat/keepalive packets

- [ ] **ARCH-002**: Separate client/server code
  - Move game logic to `shared/` module
  - Create headless server in `server/`
  - Refactor client to send inputs to server

### Milestone 3.2: Server Implementation (Week 2-3)
- [ ] **SERVER-001**: Implement headless game server
  - Accept client connections
  - Manage game rooms (1v1 matches)
  - Process player inputs
  - Validate moves (prevent cheating)

- [ ] **SERVER-002**: Implement authoritative game state
  - Server owns "true" game state
  - Process matches and attacks server-side
  - Broadcast state updates to clients

- [ ] **SERVER-003**: Add matchmaking/lobby system
  - Player queue
  - Room creation
  - Ready-up system

### Milestone 3.3: Client Networking (Week 3-4)
- [ ] **CLIENT-001**: Implement network client
  - Connect to server
  - Send player inputs
  - Receive game state updates

- [ ] **CLIENT-002**: Implement client-side prediction
  - Locally predict block movements
  - Reconcile with server state
  - Handle lag compensation

- [ ] **CLIENT-003**: Add network UI
  - Connection status indicator
  - Ping/latency display
  - Lobby/matchmaking screen
  - "Waiting for opponent" state

### Milestone 3.4: Testing & Optimization (Week 4)
- [ ] **NET-003**: Test network reliability
  - Simulate packet loss
  - Test under high latency
  - Test disconnect/reconnect

- [ ] **NET-004**: Optimize bandwidth
  - Delta compression for state updates
  - Reduce update frequency where possible
  - Measure and log bandwidth usage

**Phase 3 Success Criteria:**
- ✓ Players on different computers can play together
- ✓ Game feels responsive under normal network conditions (<100ms latency)
- ✓ Server validates all moves (no cheating possible)
- ✓ Graceful handling of disconnects
- ✓ 0 crashes in networked play

---

## Phase 4: Polish & Refinement (March 1 → April 1, 2026)
**Goal:** Make the game feel complete and professional
**Deliverable:** Polished, enjoyable game ready to share

### Milestone 4.1: Audio Implementation (Week 1-2)
- [ ] **AUDIO-001**: Add sound effects
  - Block swap sound
  - Match sound (different for 3/4/5+ matches)
  - Attack sent/received sounds
  - Cascade/combo sounds
  - Game over sound

- [ ] **AUDIO-002**: Add background music
  - Menu music
  - Gameplay music (looping)
  - Victory/defeat music
  - Music volume controls

- [ ] **AUDIO-003**: Audio polish
  - Mix sound levels
  - Ensure no audio pops/clicks
  - Add audio settings (mute, volume)

### Milestone 4.2: Visual Polish (Week 2-3)
- [ ] **VFX-001**: Add particle effects
  - Block break particles
  - Combo sparkles
  - Attack sending/receiving effects

- [ ] **VFX-002**: Improve animations
  - Smooth interpolation for all movements
  - Juice up block swaps (squash/stretch)
  - Screen shake for big attacks

- [ ] **VFX-003**: Add visual feedback
  - Damage flash when receiving attack
  - Glow effect for possible matches
  - Victory/defeat screen animations

### Milestone 4.3: Gameplay Refinement (Week 3)
- [ ] **GAME-009**: Balance tuning
  - Adjust attack strength curves
  - Tune combo multipliers
  - Test game length (should be 2-5 minutes)

- [ ] **GAME-010**: Add game modes (optional)
  - Timed mode
  - Endless mode
  - Practice mode vs AI

- [ ] **UX-001**: Improve user experience
  - Tutorial/how to play screen
  - Better menu navigation
  - Settings menu (audio, controls, graphics)

### Milestone 4.4: Cross-Platform Testing (Week 4)
- [ ] **BUILD-001**: Build and test on Windows
  - Verify compilation
  - Test executable size (<100MB)
  - Check for Windows-specific bugs

- [ ] **BUILD-002**: Build and test on macOS
  - Verify compilation
  - Test executable size
  - Check for macOS-specific bugs

- [ ] **BUILD-003**: Build and test on Linux
  - Verify compilation on Ubuntu/Debian
  - Test executable size
  - Check for Linux-specific bugs

- [ ] **BUILD-004**: Create distribution packages
  - Zip files with game + assets
  - Installation instructions
  - README with controls

**Phase 4 Success Criteria:**
- ✓ Game has sound effects and music
- ✓ Visuals are polished and satisfying
- ✓ Runs on all target platforms (Windows, macOS, Linux)
- ✓ Executable is <100MB
- ✓ Game is fun to play for 15+ minutes
- ✓ Friends/family can download and play without issues

---

## Phase 5: Nice-to-Haves (April 1 → Beyond)
**Goal:** Explore additional integrations and platforms
**Deliverable:** Enhanced features as time allows

### Optional Features (Prioritize based on interest)
- [ ] **DISCORD-001**: Discord integration
  - Rich presence (show game status)
  - Voice chat during matches
  - Discord bot for matchmaking

- [ ] **STEAM-001**: Steam integration
  - Research Steamworks SDK
  - Achievements
  - Leaderboards
  - Steam matchmaking

- [ ] **MOBILE-001**: Mobile port exploration
  - Touch controls design
  - Android/iOS build setup
  - Performance optimization for mobile

- [ ] **DECK-001**: Steam Deck optimization
  - Controller support
  - UI scaling for handheld
  - Performance testing on Deck

---

## Working with Claude Code: Best Practices

### How to Structure Tasks for Claude Code

1. **Start with Foundation Tasks**
   - Example: "Implement the game board data structure (GAME-001)"
   - Provide clear success criteria
   - Reference specific file locations

2. **Build Incrementally**
   - Don't jump ahead - complete each milestone before moving to next
   - Test each feature before continuing
   - Claude Code works best with focused, single-purpose tasks

3. **Provide Context**
   - Share relevant code files
   - Explain how new code fits into existing architecture
   - Mention dependencies between tasks

4. **Task Template for Claude Code**
   ```
   Task: [TASK-ID]: [Brief description]
   
   Context:
   - Current state: [what exists now]
   - Goal: [what this task achieves]
   - Files involved: [list relevant files]
   
   Requirements:
   - [Specific requirement 1]
   - [Specific requirement 2]
   
   Success criteria:
   - [How to verify it works]
   ```

### Suggested First Tasks

1. **ENV-001** (Setup): "Set up C project with raylib and Makefile"
2. **TEST-001** (Validation): "Create basic raylib window with FPS counter"
3. **GAME-001** (Core): "Implement game board data structure"

---

## Progress Tracking

### Phase Completion Checklist
- [ ] Phase 0: Foundation ✓
- [ ] Phase 1: Core Gameplay ✓
- [ ] Phase 2: Adversarial Features ✓
- [ ] Phase 3: Networking ✓
- [ ] Phase 4: Polish ✓
- [ ] Phase 5: Nice-to-Haves (optional)

### Key Metrics Dashboard
Track these throughout development:
- **Performance**: FPS (target: 60)
- **Stability**: Crash count (target: 0)
- **Size**: Executable size (target: <100MB)
- **Network**: Latency tolerance (target: <100ms feels good)

---

## Risk Mitigation

### Known Risks & Solutions

**Risk:** Network implementation too complex
- **Mitigation:** Start with simple TCP, migrate to UDP if needed
- **Fallback:** Local multiplayer is still valuable

**Risk:** Cross-platform compilation issues
- **Mitigation:** Test on all platforms at Phase 1 completion
- **Fallback:** Ship for one platform first, add others incrementally

**Risk:** Graphics/sound bloat executable
- **Mitigation:** Use compressed formats, minimize asset resolution
- **Fallback:** Reduce asset quality or quantity

**Risk:** Running out of time
- **Mitigation:** Each phase delivers playable game - can stop at any point
- **Fallback:** Reduce scope of Phase 5 nice-to-haves

---

## Next Steps

1. **Set up your development environment** (Phase 0)
2. **Create project structure** (ARCH-001)
3. **Verify raylib works** (TEST-001)
4. **Start Phase 1** with GAME-001

You're ready to start coding! Begin with Phase 0 tasks and work through each milestone sequentially. Each phase builds a complete, testable version of your game.

Good luck with Puzzle Attack! 🎮
