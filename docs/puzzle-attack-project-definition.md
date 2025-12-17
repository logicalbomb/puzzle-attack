# Project Definition Template

## 1. Project Overview
**Project Name:** Puzzle Attack

**Brief Description (1-2 sentences):**
Puzzle game where you match 3+ colors horizontally or vertically to reduce blocks on your side and attack your opponent with big blocks.

**Problem Statement:** What problem does this solve? Who is it for?
This is a fun project meant for entertainment. This project is also meant to tech the fundamentals of building a game with the C language. It is meant to be used by friends and family and for learning not for commercial success.

**Main Goals/Objectives:**
- Build a functional and fun puzzle game to play with friends and family.
- Learn how to build games and the systematic approach necessary to break down the problem into iterative goals.
- Learn to collaborate with AI tools for augmented development.

## 2. Current State
**Starting Point:** (choose one or describe)
- [x] Starting from scratch
- [ ] Have some existing code (describe what exists)
- [ ] Modifying/extending existing project
- [ ] Other: 

**Existing Resources:**
- Code repositories: none
- Documentation: none
- Design files: none
- APIs/Services already in use: none
  - However, depending on the time eventually I would like to integrate with Discord game/chat APIs and Steam APIs as well.

## 3. Technical Details

**Type of Project:** (web app, mobile app, CLI tool, API, library, etc.)
This is a video game targeted at computers running Windows or OSX to start. After the game is complete targeting mobile phones or the Steam Deck would be fun.

**Tech Stack:**
- Language(s): C
- Framework(s): custom
- Database: custom files
- Key libraries/dependencies: raylib, maybe curl
- Hosting/deployment: self-hosted, maybe steam in the future

**Architecture/Structure:**
(Brief description of how components fit together)
The game is going to have a client/server model in place. The server is in charge of owning and validating the game state, player moves and prevent tampering cheating. The server runs in a headless mode and will establish network connections with player clients over reliable UDP (similar to Quake 3 Arena). The client runs a loop that will constantly gather inputs, communicate with the server, predict and reconcile local game state, and render that game state.

**External Integrations:**
- APIs: maybe Discord and Steam, these are nice to have and lower priority
- Third-party services: see above
- Authentication: none planned unless necessary

## 4. Features & Scope

**Must-Have Features (MVP):**
1. A simple match 3 game that renders properly.
2. Adversarial play features (attacking each other).
3. Networking support to allow for people to play each other from different devices.
4. Sound effects and music to accompany the game.
5. Work on computer platforms (OSX, Windows, Linux).

**Nice-to-Have Features (Future):**
1. Integration with Discord to allow for people to chat/speak while playing.
2. Integration with the Steam environment to understand what the platform provides.
3. Extension to more platforms like mobile phones and Steam Deck.

**Explicitly Out of Scope:**
- Sales and money gathering.
- Targeting console platforms.

## 5. Timeline & Constraints

**Target Launch Date:** (or "no specific deadline")
No specific deadlines, but would like to have an MVP working by January 1, 2026.

**Key Milestones/Deadlines:**
- Simplified MVP of the game working by January 1, 2026.
- Networked version of the game functional by February 1, 2026.
- Refined version of the game by March 1, 2026.
- Explore and implement nice-to-haves by April 1, 2026.

**Time Availability:** (e.g., "10 hours/week", "full-time", etc.)
Varies 10 hours/week most weeks, sometimes upwards of 20 hours/week.

**Hard Constraints:**
- Budget limitations: no budget, build it not buy it
- Technical requirements: use C, ensure that it can support the known platforms, only use necessary libraries
- Compliance/legal requirements: none
- Other: none

## 6. Success Criteria

**How will you know this project is successful?**
- If I can play the game with friends and family and have fun with the game.
- The game does not crash under various play conditions.
- I feel confident in building other games given this experience.
- I feel comfortable with relying upon AI assitants to deliver tasks in a larger project.
- The game runs smoothly and does not have any visual lag that interferes with a players ability to play.
- Small executable size to make it easy to share with friends and family.

**Key Metrics:**
- 0 runtime crashes of client and server
- 60 frames drawn per second during gameplay
- Executable size no larger than 100 MB

## 7. Risks & Concerns

**What are you worried about?**
- Discovery of other players and game matching when used by non-technical players.
- Graphics and sound quality and size tradeoff.
- Overall time it takes to build the project.

**Known Challenges:**
- Lack of experience in building similar projects.

## 8. Additional Context

**Target Users/Audience:**

**Similar Projects/Inspiration:**

**Any other relevant information:**
