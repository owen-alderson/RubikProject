#include "solver.h"
#include <string.h>

// ---------------- Global solution buffer ----------------

Move SOLUTION_MOVES[MAX_SOLUTION_MOVES];
int  SOLUTION_LEN = 0;

// ---------------- Scramble history ----------------------

static Move SCRAMBLE_MOVES[MAX_SCRAMBLE_MOVES];
static int  SCRAMBLE_LEN = 0;

void solver_reset_history(void) {
    SCRAMBLE_LEN = 0;
}

void solver_record_scramble_move(Move m) {
    if (SCRAMBLE_LEN < MAX_SCRAMBLE_MOVES) {
        SCRAMBLE_MOVES[SCRAMBLE_LEN++] = m;
    }
}

// ---------------- Move helpers --------------------------

const char *move_to_str(Move m) {
    switch (m) {
        case MOVE_U:        return "U";
        case MOVE_U_PRIME:  return "U'";
        case MOVE_U2:       return "U2";
        case MOVE_D:        return "D";
        case MOVE_D_PRIME:  return "D'";
        case MOVE_D2:       return "D2";
        case MOVE_F:        return "F";
        case MOVE_F_PRIME:  return "F'";
        case MOVE_F2:       return "F2";
        case MOVE_B:        return "B";
        case MOVE_B_PRIME:  return "B'";
        case MOVE_B2:       return "B2";
        case MOVE_L:        return "L";
        case MOVE_L_PRIME:  return "L'";
        case MOVE_L2:       return "L2";
        case MOVE_R:        return "R";
        case MOVE_R_PRIME:  return "R'";
        case MOVE_R2:       return "R2";
        case MOVE_NONE:     return "-";
        default:            return "?";
    }
}

static Move inverse_move(Move m) {
    switch (m) {
        case MOVE_U:        return MOVE_U_PRIME;
        case MOVE_U_PRIME:  return MOVE_U;
        case MOVE_D:        return MOVE_D_PRIME;
        case MOVE_D_PRIME:  return MOVE_D;
        case MOVE_F:        return MOVE_F_PRIME;
        case MOVE_F_PRIME:  return MOVE_F;
        case MOVE_B:        return MOVE_B_PRIME;
        case MOVE_B_PRIME:  return MOVE_B;
        case MOVE_L:        return MOVE_L_PRIME;
        case MOVE_L_PRIME:  return MOVE_L;
        case MOVE_R:        return MOVE_R_PRIME;
        case MOVE_R_PRIME:  return MOVE_R;
        case MOVE_U2:       return MOVE_U2;
        case MOVE_D2:       return MOVE_D2;
        case MOVE_F2:       return MOVE_F2;
        case MOVE_B2:       return MOVE_B2;
        case MOVE_L2:       return MOVE_L2;
        case MOVE_R2:       return MOVE_R2;
        case MOVE_NONE:     return MOVE_NONE;
        default:            return MOVE_NONE;
    }
}

// ---------------- Solver core ---------------------------

bool solver_solve(Cube *cube, int max_depth_unused) {
    (void)max_depth_unused; // parameter kept only for compatibility

    SOLUTION_LEN = 0;

    // If cube is already solved, nothing to do
    if (cube_is_solved(cube)) {
        return true;
    }

    // If we have no scramble history, we can't reverse it
    if (SCRAMBLE_LEN == 0) {
        // No guarantee in this case; just say "no solution"
        // You could later plug a heuristic solver here if you want.
        return false;
    }

    // Apply inverse of the recorded scramble, in reverse order
    for (int i = SCRAMBLE_LEN - 1; i >= 0; --i) {
        Move inv = inverse_move(SCRAMBLE_MOVES[i]);
        cube_apply_move(cube, inv);

        if (SOLUTION_LEN < MAX_SOLUTION_MOVES) {
            SOLUTION_MOVES[SOLUTION_LEN++] = inv;
        }
    }

    // This *should* be solved if cube started as solved and only
    // had those scramble moves applied.
    return cube_is_solved(cube);
}

// ---------------- Hint interface ------------------------

// For now, the "best hint" is simply the next inverse scramble move.
// If you haven't made any user moves after scrambling, this is
// literally the optimal move: you are undoing the scramble.
Move solver_best_hint(const Cube *cube) {
    (void)cube; // we don't actually inspect the cube here

    if (SCRAMBLE_LEN == 0) {
        return MOVE_NONE;
    }

    // If you are still in the original scrambled state, the "first"
    // undo move is the inverse of the *last* scramble move.
    Move last_scramble = SCRAMBLE_MOVES[SCRAMBLE_LEN - 1];
    return inverse_move(last_scramble);
}
