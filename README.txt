# Geometry Wars Style Shooter

This project is a small C++ game built with SFML that follows the structure of a classic arcade shooter inspired by Geometry Wars. The player controls a polygonal ship, moves around the screen, fires at incoming enemies, and tries to survive as long as possible while building up a score.

## Project Overview

The game reads its settings from `config.txt`, creates the main game window, spawns the player and enemy entities, and updates the simulation each frame. It uses an entity-component style structure with separate managers for entities, shapes, transforms, collision, scoring, and lifespan effects.

## Current Gameplay

### Player controls
- Move: `W`, `A`, `S`, `D`
- Aim: mouse pointer
- Shoot: left mouse button
- Pause: `P`
- Exit: `Esc`

The player spawns in the center of the screen and is kept inside the window bounds. Bullets travel toward the mouse cursor and are removed after their configured lifespan expires.

### Enemies
- Enemies spawn at random positions on the screen.
- They move with random velocity and bounce off the screen edges.
- They rotate slowly while active.
- When large enemies are destroyed, they split into smaller enemies that fly outward in a fan pattern.

### Score system
- Each enemy has a score based on its number of vertices.
- Destroying enemies increases the score shown in the upper-left corner of the screen.
- The game score is updated via the text renderer using the configured font.

### Special weapon
A special weapon hook exists in the code (`spawnSpecialWeapon`), but the current implementation is scaffolded rather than fully completed. The assignment specification mentions a right-click special move with cooldown and multiple spawned projectiles, but that feature is not fully implemented in the present version.

## Configuration

The game setup is controlled by the file `config.txt`:
- window size and frame limit
- font path and color
- player settings
- enemy settings
- bullet settings

The code also expects the font file `fonts/arial.ttf` to be available in the project.

## Project Structure

- `main.cpp` – entry point for the game
- `Game.h` / `Game.cpp` – primary game loop, systems, spawning logic, collision handling, and rendering
- `Entity.h` / `Entity.cpp` – entity representation and lifecycle
- `EntityManager.h` / `EntityManager.cpp` – entity creation and cleanup
- `Component.h` – component definitions for transform, shape, input, scoring, and lifespan
- `vec2.h` / `vec2.cpp` – vector math utility used by the game
- `config.txt` – runtime configuration values
- `fonts/arial.ttf` – font used for the score display

## Build and Run

This project is a Visual Studio C++ solution (`Assignment2.sln`). To run it:

1. Open `Assignment2.sln` in Visual Studio.
2. Ensure the SFML dependencies are correctly configured for your environment.
3. Build the solution.
4. Run the project.

If the environment is set up correctly, the game window should open and begin running immediately.

## Notes

This project is a coursework assignment and includes a mix of completed and partially completed systems. The main gameplay loop, rendering, player controls, and enemy spawning are implemented, while some advanced mechanics such as the special weapon remain a future improvement.
