# Conway's Game of Life in C
## A terminal-based implementation of Conway's cellular automaton using ncurses for real-time visualization.


### Project Overview

This C implementation of Conway's Game of Life was developed as a collaborative academic project in a team of three. The project features an interactive terminal interface with five preset patterns and adjustable simulation speed


### My Contribution

* User Interface (user_desire): interactive pattern selection menu with input validation to ensure only valid pattern selections (1-5) are accepted. Invalid input safely terminates the program with an error flag, preventing undefined behavior and potential crashes.
* File I/O System (load_file): loading and parsing pattern files with error handling at both file opening (fopen) and reading stages (fgets)
* Core Algorithm (count_neighbors): collaborative implementation of toroidal boundary logic using modular arithmetic (`(position + size) % size`)


### Quick start

*Prerequisites*:
* GCC compiler
* ncurses library
* Linux/macOS terminal (or WSL on Windows)

```bash
gcc -o game game_of_life.c -lncurses && ./game
```


### Technical Stack
Tech Stack:
C - NCurses - Git - Linux Development Environment


### Features

* Terminal-based visualization using ncurses library
* Five patterns: Achim's p4, random, Pond, Turtle, and Orion
* Real-time controls: adjustable speed (50-500ms per generation)
* Toroidal world: edges wrap around (cyclic boundaries)
* Clean console interface with visual feedback


### Technical Challenges & Solutions

- Real-time input/output balance: the simulation loop required careful timing between updating cell states and refreshing the display. Using ncurses
`nodelay()` mode allowed user input (speed adjustment, exit) to be processed without pausing the simulation, while `napms()` controlled the generation speed.
- Memory architecture decision: we chose fixed-size arrays (`int[25][80]`) over dynamic allocation for the cell grids. This simplified memory management but limited flexibility - the grid size is hardcoded and cannot be changed at runtime. Now I understand the importance of upfront design decisions.
- Platform dependency reality check: while ncurses is cross-platform in theory, in practice Windows compatibility requires additional layers like Cygwin or WSL. This was an unexpected constraint. There is a huge difference between theoretical and practical portability.


### Control

| Key       | Action                              |
|-----------|-------------------------------------|
| 1-5       | Select initial pattern (at startup) |
| A         | Increase simulation speed           | 
| Z         | Decrease simulation speed           |
| SPACE     | Exit program                        |


### Technical Implementation

**Core Algorithm**
The implementation follows Conway's original rules with optimizations for performance:

1. Dual buffering: uses `cells` and `next` arrays to avoid in-place updates
2. Neighbor counting: 8-connected neighbourhood with modular arithmetic for toroidal edges
3. State transition: classic B3/S23 rules (Birth on 3, Survival on 2 or 3)


### Key Functions:

```bash
int count_neighbors(int cells[HEIGHT][WIDTH], int y, int x);  // modular counting
void update(int cells[HEIGHT][WIDTH], int next[HEIGHT][WIDTH]); // state transition
void draw_cells(int cells[HEIGHT][WIDTH], int speed);         // ncurses rendering
int load_file(int cells[HEIGHT][WIDTH], const char* filename);   // pattern loading
```


### My Engineering Insights from Collaboration

This project showed that in development you need to find balance. We chose an 80x25 grid for a reason - it's a convenient terminal size that both looks good and runs fast. When we were working together on the neighbor-counting algorithm, we realized: the same task can be solved in different ways. You can write code that's short and clear, or you can unroll it for speed. The main thing is that the parts of the program need to work together seamlessly - that makes teamwork much easier.