#include "game.h"
#include "cube.h"
#include "solver.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#define sleep_ms(ms) Sleep(ms)
#else
#include <unistd.h>
#define sleep_ms(ms) usleep((ms) * 1000)
#endif


// =====================================================
//                     UTILITIES
// =====================================================

static Move parse_move(const char *s) {
    if (strcmp(s, "U") == 0)  return MOVE_U;
    if (strcmp(s, "U'") == 0) return MOVE_U_PRIME;
    if (strcmp(s, "U2") == 0) return MOVE_U2;

    if (strcmp(s, "D") == 0)  return MOVE_D;
    if (strcmp(s, "D'") == 0) return MOVE_D_PRIME;
    if (strcmp(s, "D2") == 0) return MOVE_D2;

    if (strcmp(s, "F") == 0)  return MOVE_F;
    if (strcmp(s, "F'") == 0) return MOVE_F_PRIME;
    if (strcmp(s, "F2") == 0) return MOVE_F2;

    if (strcmp(s, "B") == 0)  return MOVE_B;
    if (strcmp(s, "B'") == 0) return MOVE_B_PRIME;
    if (strcmp(s, "B2") == 0) return MOVE_B2;

    if (strcmp(s, "L") == 0)  return MOVE_L;
    if (strcmp(s, "L'") == 0) return MOVE_L_PRIME;
    if (strcmp(s, "L2") == 0) return MOVE_L2;

    if (strcmp(s, "R") == 0)  return MOVE_R;
    if (strcmp(s, "R'") == 0) return MOVE_R_PRIME;
    if (strcmp(s, "R2") == 0) return MOVE_R2;

    return MOVE_NONE;
}

static void apply_algorithm(Cube *cube, char *line) {
    char *tok = strtok(line, " ");
    while (tok) {
        Move m = parse_move(tok);
        if (m == MOVE_NONE)
            printf("  Invalid move: %s\n", tok);
        else
            cube_apply_move(cube, m);

        tok = strtok(NULL, " ");
    }
}


// =====================================================
//                     MENU HEADER
// =====================================================

static void print_menu_header() {
    printf("\n=========================================\n");
    printf("          RUBIK’S CUBE SYSTEM v2.0\n");
    printf("=========================================\n");
    printf("  Commands:\n");
    printf("    scramble N     → Scramble with N moves\n");
    printf("    print          → Display cube\n");
    printf("    solve          → Solve (threaded, timed)\n");
    printf("    hint           → Smart next-move hint\n");
    printf("    save           → Save cube to cube.txt\n");
    printf("    load           → Load cube from cube.txt\n");
    printf("    reset          → Reset to solved\n");
    printf("    moves...       → Apply moves (U R' F2 ...)\n");
    printf("    quit           → Exit program\n");
    printf("=========================================\n\n");
}


// =====================================================
//                  ANIMATED SOLUTION
// =====================================================

void animate_solution(Cube *cube) {
    printf("\nAnimating solution:\n");

    for (int i = 0; i < SOLUTION_LEN; i++) {
        printf("  %s\n", move_to_str(SOLUTION_MOVES[i]));
        cube_apply_move(cube, SOLUTION_MOVES[i]);
        cube_print(cube);
        sleep_ms(300);
    }
}


// =====================================================
//                      GAME LOOP
// =====================================================

void game_run() {
    Cube cube;
    cube_init(&cube);

    print_menu_header();
    cube_print(&cube);

    char line[256];

    while (1) {
        printf("> ");
        if (!fgets(line, sizeof(line), stdin))
            break;

        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0)
            continue;

        // reset
        if (strcmp(line, "reset") == 0) {
            cube_init(&cube);
            printf("Cube reset to solved.\n");
            cube_print(&cube);
            continue;
        }

        // quit
        if (strcmp(line, "quit") == 0)
            break;

        // print
        if (strcmp(line, "print") == 0) {
            cube_print(&cube);
            continue;
        }

        // save
        if (strcmp(line, "save") == 0) {
            if (cube_save(&cube, "cube.txt"))
                printf("State saved to cube.txt\n");
            else
                printf("Save failed.\n");
            continue;
        }

        // load
        if (strcmp(line, "load") == 0) {
            if (cube_load(&cube, "cube.txt")) {
                printf("Loaded cube from cube.txt\n");
                cube_print(&cube);
            } else {
                printf("Load failed (invalid cube or missing file).\n");
            }
            continue;
        }

        // solve
        if (strcmp(line, "solve") == 0) {
            printf("Solving using heuristic search (max 200 steps)...\n");

            clock_t start = clock();

            int max_steps = 200; // you can tweak this
            bool ok = solver_solve(&cube, max_steps);

            clock_t end = clock();
            double ms = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
            printf("Time: %.2f ms\n", ms);

            if (ok) {
                printf("Solution (%d moves): ", SOLUTION_LEN);
                for (int i = 0; i < SOLUTION_LEN; i++)
                    printf("%s ", move_to_str(SOLUTION_MOVES[i]));
                printf("\n");
                cube_print(&cube);
            } else {
                printf("Gave up after %d steps; cube might be close but not fully solved.\n", max_steps);
                cube_print(&cube);
            }
            continue;
        }



        // hint
        if (strcmp(line, "hint") == 0) {
            printf("Computing optimal hint (depth 3)...\n");
            clock_t start = clock();

            Move m = solver_best_hint(&cube);

            clock_t end = clock();
            double ms = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

            if (m == MOVE_NONE) {
                printf("No helpful hint found.\n");
            } else {
                printf("Hint: Try %s    (%.2f ms)\n", move_to_str(m), ms);
            }

            continue;
        }


        // scramble
        if (strncmp(line, "scramble", 8) == 0) {
            int n = atoi(line + 9);
            if (n <= 0) n = 20;

            printf("Scrambling %d moves...\n", n);
            cube_scramble(&cube, n);
            cube_print(&cube);
            continue;
        }

        // Otherwise interpret as moves
        apply_algorithm(&cube, line);
        cube_print(&cube);

        if (cube_is_solved(&cube))
            printf("Cube SOLVED!\n");
    }
}
