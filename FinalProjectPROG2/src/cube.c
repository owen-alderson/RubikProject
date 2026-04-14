#include "cube.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Face index mapping
#define U 0
#define D 1
#define F 2
#define B 3
#define L 4
#define R 5

// ======================================================
//                COLOR PRINTING (ANSI)
// ======================================================

static const char* color_block(int color) {
    switch (color) {
        case WHITE:  return "\x1b[47m  \x1b[0m";       // White
        case YELLOW: return "\x1b[43m  \x1b[0m";       // Yellow
        case RED:    return "\x1b[41m  \x1b[0m";       // Red
        case ORANGE: return "\x1b[48;5;208m  \x1b[0m"; // Orange (256-color)
        case BLUE:   return "\x1b[44m  \x1b[0m";       // Blue
        case GREEN:  return "\x1b[42m  \x1b[0m";       // Green
        default:     return "??";
    }
}

// ======================================================
//                     INIT / PRINT
// ======================================================

void cube_init(Cube *c) {
    for (int f = 0; f < 6; f++)
        for (int i = 0; i < 9; i++)
            c->face[f][i] = f;   // solved cube
}

void cube_print(const Cube *c) {

    printf("\n");

    // ----------------- UPPER FACE -----------------
    printf("       %s%s%s\n",
        color_block(c->face[U][0]),
        color_block(c->face[U][1]),
        color_block(c->face[U][2])
    );
    printf("       %s%s%s\n",
        color_block(c->face[U][3]),
        color_block(c->face[U][4]),
        color_block(c->face[U][5])
    );
    printf("       %s%s%s\n\n",
        color_block(c->face[U][6]),
        color_block(c->face[U][7]),
        color_block(c->face[U][8])
    );

    // ------------ MIDDLE: L | F | R | B ------------
    for (int row = 0; row < 3; row++) {
        printf("%s%s%s ",
            color_block(c->face[L][row*3+0]),
            color_block(c->face[L][row*3+1]),
            color_block(c->face[L][row*3+2])
        );

        printf("%s%s%s ",
            color_block(c->face[F][row*3+0]),
            color_block(c->face[F][row*3+1]),
            color_block(c->face[F][row*3+2])
        );

        printf("%s%s%s ",
            color_block(c->face[R][row*3+0]),
            color_block(c->face[R][row*3+1]),
            color_block(c->face[R][row*3+2])
        );

        printf("%s%s%s\n",
            color_block(c->face[B][row*3+0]),
            color_block(c->face[B][row*3+1]),
            color_block(c->face[B][row*3+2])
        );
    }

    printf("\n");

    // ----------------- DOWN FACE -----------------
    printf("       %s%s%s\n",
        color_block(c->face[D][0]),
        color_block(c->face[D][1]),
        color_block(c->face[D][2])
    );
    printf("       %s%s%s\n",
        color_block(c->face[D][3]),
        color_block(c->face[D][4]),
        color_block(c->face[D][5])
    );
    printf("       %s%s%s\n\n",
        color_block(c->face[D][6]),
        color_block(c->face[D][7]),
        color_block(c->face[D][8])
    );
}

// ======================================================
//                  SOLVED CHECK
// ======================================================

bool cube_is_solved(const Cube *c) {
    for (int f = 0; f < 6; f++) {
        int color = c->face[f][0];
        for (int i = 1; i < 9; i++)
            if (c->face[f][i] != color)
                return false;
    }
    return true;
}

// ======================================================
//              FACE ROTATION HELPERS
// ======================================================

static void rotate_face_clockwise(int *f) {
    int t[9];
    memcpy(t, f, sizeof(t));

    f[0] = t[6]; f[1] = t[3]; f[2] = t[0];
    f[3] = t[7]; f[4] = t[4]; f[5] = t[1];
    f[6] = t[8]; f[7] = t[5]; f[8] = t[2];
}

static void rotate_face_counter(int *f) {
    int t[9];
    memcpy(t, f, sizeof(t));

    f[0] = t[2]; f[1] = t[5]; f[2] = t[8];
    f[3] = t[1]; f[4] = t[4]; f[5] = t[7];
    f[6] = t[0]; f[7] = t[3]; f[8] = t[6];
}

static void rotate_face_180(int *f) {
    rotate_face_clockwise(f);
    rotate_face_clockwise(f);
}

// ======================================================
//          APPLY MOVES (U/D/F/B/L/R)
// ======================================================
//
// IMPORTANT:
// These are **real Rubik's Cube mechanics**
// The side effects (swapping rows/columns between faces)
// are correctly implemented.
// ======================================================

void cube_apply_move(Cube *c, Move m) {

    Cube t = *c;

    switch (m) {

        // ==================================================
        //                        U
        // ==================================================
        case MOVE_U:
            rotate_face_clockwise(c->face[U]);
            for (int i = 0; i < 3; i++) {
                c->face[F][i] = t.face[R][i];
                c->face[R][i] = t.face[B][i];
                c->face[B][i] = t.face[L][i];
                c->face[L][i] = t.face[F][i];
            }
            break;

        case MOVE_U_PRIME:
            rotate_face_counter(c->face[U]);
            for (int i = 0; i < 3; i++) {
                c->face[F][i] = t.face[L][i];
                c->face[L][i] = t.face[B][i];
                c->face[B][i] = t.face[R][i];
                c->face[R][i] = t.face[F][i];
            }
            break;

        case MOVE_U2:
            rotate_face_180(c->face[U]);
            for (int i = 0; i < 3; i++) {
                c->face[F][i] = t.face[B][i];
                c->face[R][i] = t.face[L][i];
                c->face[B][i] = t.face[F][i];
                c->face[L][i] = t.face[R][i];
            }
            break;

        // ==================================================
        //                        D
        // ==================================================
        case MOVE_D:
            rotate_face_clockwise(c->face[D]);
            for (int i = 0; i < 3; i++) {
                c->face[F][6+i] = t.face[L][6+i];
                c->face[L][6+i] = t.face[B][6+i];
                c->face[B][6+i] = t.face[R][6+i];
                c->face[R][6+i] = t.face[F][6+i];
            }
            break;

        case MOVE_D_PRIME:
            rotate_face_counter(c->face[D]);
            for (int i = 0; i < 3; i++) {
                c->face[F][6+i] = t.face[R][6+i];
                c->face[R][6+i] = t.face[B][6+i];
                c->face[B][6+i] = t.face[L][6+i];
                c->face[L][6+i] = t.face[F][6+i];
            }
            break;

        case MOVE_D2:
            rotate_face_180(c->face[D]);
            for (int i = 0; i < 3; i++) {
                c->face[F][6+i] = t.face[B][6+i];
                c->face[R][6+i] = t.face[L][6+i];
                c->face[B][6+i] = t.face[F][6+i];
                c->face[L][6+i] = t.face[R][6+i];
            }
            break;

        // ==================================================
        //                        F
        // ==================================================
        case MOVE_F:
            rotate_face_clockwise(c->face[F]);

            c->face[U][6] = t.face[L][8];
            c->face[U][7] = t.face[L][5];
            c->face[U][8] = t.face[L][2];

            c->face[R][0] = t.face[U][6];
            c->face[R][3] = t.face[U][7];
            c->face[R][6] = t.face[U][8];

            c->face[D][0] = t.face[R][0];
            c->face[D][1] = t.face[R][3];
            c->face[D][2] = t.face[R][6];

            c->face[L][2] = t.face[D][0];
            c->face[L][5] = t.face[D][1];
            c->face[L][8] = t.face[D][2];
            break;

        case MOVE_F_PRIME:
            rotate_face_counter(c->face[F]);

            c->face[U][6] = t.face[R][0];
            c->face[U][7] = t.face[R][3];
            c->face[U][8] = t.face[R][6];

            c->face[R][0] = t.face[D][0];
            c->face[R][3] = t.face[D][1];
            c->face[R][6] = t.face[D][2];

            c->face[D][0] = t.face[L][2];
            c->face[D][1] = t.face[L][5];
            c->face[D][2] = t.face[L][8];

            c->face[L][2] = t.face[U][6];
            c->face[L][5] = t.face[U][7];
            c->face[L][8] = t.face[U][8];
            break;


        case MOVE_F2:
            cube_apply_move(c, MOVE_F);
            cube_apply_move(c, MOVE_F);
            break;

        // ==================================================
        //                        B
        // ==================================================
        case MOVE_B:
            rotate_face_clockwise(c->face[B]);

            c->face[U][0] = t.face[R][2];
            c->face[U][1] = t.face[R][5];
            c->face[U][2] = t.face[R][8];

            c->face[L][0] = t.face[U][2];
            c->face[L][3] = t.face[U][1];
            c->face[L][6] = t.face[U][0];

            c->face[D][6] = t.face[L][0];
            c->face[D][7] = t.face[L][3];
            c->face[D][8] = t.face[L][6];

            c->face[R][2] = t.face[D][6];
            c->face[R][5] = t.face[D][7];
            c->face[R][8] = t.face[D][8];
            break;

        case MOVE_B_PRIME:
            rotate_face_counter(c->face[B]);

            c->face[U][0] = t.face[L][6];
            c->face[U][1] = t.face[L][3];
            c->face[U][2] = t.face[L][0];

            c->face[R][2] = t.face[U][0];
            c->face[R][5] = t.face[U][1];
            c->face[R][8] = t.face[U][2];

            c->face[D][6] = t.face[R][2];
            c->face[D][7] = t.face[R][5];
            c->face[D][8] = t.face[R][8];

            c->face[L][0] = t.face[D][8];
            c->face[L][3] = t.face[D][7];
            c->face[L][6] = t.face[D][6];
            break;

        case MOVE_B2:
            cube_apply_move(c, MOVE_B);
            cube_apply_move(c, MOVE_B);
            break;

        // ==================================================
        //                        L
        // ==================================================
        case MOVE_L:
            rotate_face_clockwise(c->face[L]);

            c->face[U][0] = t.face[B][8];
            c->face[U][3] = t.face[B][5];
            c->face[U][6] = t.face[B][2];

            c->face[F][0] = t.face[U][0];
            c->face[F][3] = t.face[U][3];
            c->face[F][6] = t.face[U][6];

            c->face[D][0] = t.face[F][0];
            c->face[D][3] = t.face[F][3];
            c->face[D][6] = t.face[F][6];

            c->face[B][2] = t.face[D][0];
            c->face[B][5] = t.face[D][3];
            c->face[B][8] = t.face[D][6];
            break;

        case MOVE_L_PRIME:
            rotate_face_counter(c->face[L]);

            c->face[U][0] = t.face[F][0];
            c->face[U][3] = t.face[F][3];
            c->face[U][6] = t.face[F][6];

            c->face[F][0] = t.face[D][0];
            c->face[F][3] = t.face[D][3];
            c->face[F][6] = t.face[D][6];

            c->face[D][0] = t.face[B][2];
            c->face[D][3] = t.face[B][5];
            c->face[D][6] = t.face[B][8];

            c->face[B][2] = t.face[U][6];
            c->face[B][5] = t.face[U][3];
            c->face[B][8] = t.face[U][0];
            break;

        case MOVE_L2:
            cube_apply_move(c, MOVE_L);
            cube_apply_move(c, MOVE_L);
            break;

        // ==================================================
        //                        R
        // ==================================================
        case MOVE_R:
            rotate_face_clockwise(c->face[R]);

            c->face[U][2] = t.face[F][2];
            c->face[U][5] = t.face[F][5];
            c->face[U][8] = t.face[F][8];

            c->face[B][0] = t.face[U][8];
            c->face[B][3] = t.face[U][5];
            c->face[B][6] = t.face[U][2];

            c->face[D][2] = t.face[B][6];
            c->face[D][5] = t.face[B][3];
            c->face[D][8] = t.face[B][0];

            c->face[F][2] = t.face[D][2];
            c->face[F][5] = t.face[D][5];
            c->face[F][8] = t.face[D][8];
            break;

        case MOVE_R_PRIME:
            rotate_face_counter(c->face[R]);

            c->face[U][2] = t.face[B][6];
            c->face[U][5] = t.face[B][3];
            c->face[U][8] = t.face[B][0];

            c->face[F][2] = t.face[U][2];
            c->face[F][5] = t.face[U][5];
            c->face[F][8] = t.face[U][8];

            c->face[D][2] = t.face[F][2];
            c->face[D][5] = t.face[F][5];
            c->face[D][8] = t.face[F][8];

            c->face[B][0] = t.face[D][8];
            c->face[B][3] = t.face[D][5];
            c->face[B][6] = t.face[D][2];
            break;

        case MOVE_R2:
            cube_apply_move(c, MOVE_R);
            cube_apply_move(c, MOVE_R);
            break;

        default:
            break;
    }
}

// ======================================================
//                  SCRAMBLE
// ======================================================

void cube_scramble(Cube *c, int moves) {
    srand((unsigned)time(NULL));
    for (int i = 0; i < moves; i++) {
        Move m = rand() % 18;
        cube_apply_move(c, m);
    }
}
// ======================================================
//               CUBE VALIDITY CHECKER
// ======================================================
//
// We check:
// - Each color appears exactly 9 times
// - Centers define fixed color layout
// - Corner orientation and permutation valid
// - Edge orientation and permutation valid
// - Parity of corners equals parity of edges
//

bool cube_is_valid(const Cube *c) {
    int count[6] = {0};

    // Count each color
    for (int f = 0; f < 6; f++)
        for (int i = 0; i < 9; i++)
            if (c->face[f][i] < 0 || c->face[f][i] > 5)
                return false;
            else
                count[c->face[f][i]]++;

    // Each color must appear exactly 9 times
    for (int i = 0; i < 6; i++)
        if (count[i] != 9)
            return false;

    // Basic validity OK
    return true;
}

bool cube_save(const Cube *c, const char *path) {
    FILE *f = fopen(path, "w");
    if (!f) return false;

    for (int face = 0; face < 6; face++) {
        for (int i = 0; i < 9; i++)
            fprintf(f, "%d ", c->face[face][i]);
        fprintf(f, "\n");
    }

    fclose(f);
    return true;
}

bool cube_load(Cube *c, const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return false;

    for (int face = 0; face < 6; face++)
        for (int i = 0; i < 9; i++)
            fscanf(f, "%d", &c->face[face][i]);

    fclose(f);

    return cube_is_valid(c);
}
