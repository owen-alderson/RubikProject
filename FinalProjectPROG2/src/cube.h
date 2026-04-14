#ifndef CUBE_H
#define CUBE_H

#include <stdbool.h>

#define CUBE_FACES 6
#define FACE_SIZE 9

// Color definitions
typedef enum {
    WHITE = 0,  // Up
    YELLOW,     // Down
    RED,        // Front
    ORANGE,     // Back
    BLUE,       // Left
    GREEN       // Right
} Color;

// Moves
typedef enum {
    MOVE_U, MOVE_U_PRIME, MOVE_U2,
    MOVE_D, MOVE_D_PRIME, MOVE_D2,
    MOVE_F, MOVE_F_PRIME, MOVE_F2,
    MOVE_B, MOVE_B_PRIME, MOVE_B2,
    MOVE_L, MOVE_L_PRIME, MOVE_L2,
    MOVE_R, MOVE_R_PRIME, MOVE_R2,
    MOVE_NONE
} Move;

typedef struct {
    int face[CUBE_FACES][FACE_SIZE]; // each sticker holds a color (0–5)
} Cube;

void cube_init(Cube *c);                   // solved cube
void cube_print(const Cube *c);            // print in net layout
bool cube_is_solved(const Cube *c);        // check if solved
void cube_apply_move(Cube *c, Move move);  // apply one move
void cube_scramble(Cube *c, int moves);    // random scramble
bool cube_is_valid(const Cube *c);
bool cube_save(const Cube *c, const char *path);
bool cube_load(Cube *c, const char *path);


#endif
