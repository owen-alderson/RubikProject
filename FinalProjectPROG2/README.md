# Rubik’s Cube Solver (C • Multithreaded • Animated)

This project is a complete Rubik’s Cube engine and solver written entirely in ANSI C.  
It features a fully interactive terminal UI, color-rendered cube, multithreaded search,
smart hinting, animated solving, and save/load functionality.

---

## 🚀 Features

### 🧩 Cube Engine
- Accurate 3×3 Rubik’s Cube model (`6 faces × 9 stickers`)
- Realistic face-turn mechanics for all 18 moves (U, U’, U2, D, F, B, L, R)
- ANSI-colored cube rendering in the terminal
- Cube validity checking (ensures states are physically possible)

### 🤖 Solver
- **Multithreaded DFS solver** (each thread searches a different branch)
- **Iterative Deepening (IDDFS)** up to depth 12
- Pruning to reduce redundant moves
- Timed solving output (ms precision)
- Animated solving (shows each move with delay)

### 🧠 Smart Hint System
Evaluates all 18 moves, performs depth-limited searches, and chooses the move that
brings the cube closest to solved state. This is a real analytical hint.

### 💾 Save & Load
- Save cube state → `cube.txt`
- Load cube state → `cube.txt`
- Validity check ensures loaded cube is legal

### 🎮 Interactive CLI

Commands available inside the program:

scramble N → scramble with N random moves
solve → solve the cube (threaded + timed)
hint → best next move (threaded)
print → show cube in color
save → save cube state to cube.txt
load → load cube state from cube.txt
reset → return to solved state
quit → exit program
U R' F2 ... → manually apply moves to the cube