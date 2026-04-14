#ifndef SOLVER_H
#define SOLVER_H

#include "cube.h"
#include <stdbool.h>

#define MAX_SOLUTION_MOVES 256
#define MAX_SCRAMBLE_MOVES 256

// Global buffer with the last computed solution
extern Move SOLUTION_MOVES[MAX_SOLUTION_MOVES];
extern int  SOLUTION_LEN;

// Convert a move to a string like "U", "R'", "F2"
const char *move_to_str(Move m);

// --- Scramble history interface ---------------------------------
// Call this BEFORE doing a new scramble.
void solver_reset_history(void);

// Call this ONCE for every random scramble move you apply.
// (Typically from inside cube_scramble.)
void solver_record_scramble_move(Move m);

// --- Solving -----------------------------------------------------
// Solve by reversing the recorded scramble moves.
// 'max_depth' is ignored but kept for compatibility.
// Fills SOLUTION_MOVES / SOLUTION_LEN.
// Returns true if the cube ends up solved.
bool solver_solve(Cube *cube, int max_depth);

// --- Hint interface ----------------------------------------------
// Best hint = first move of the "reverse scramble" sequence, if any.
// If no scramble history is available, returns MOVE_NONE.
Move solver_best_hint(const Cube *cube);

#endif
