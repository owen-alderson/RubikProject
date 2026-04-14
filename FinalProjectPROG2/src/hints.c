#include "hints.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *compute_hint_string(const Cube *cube) {
    if (!cube) return NULL;

    Move m = solver_best_hint(cube);
    char buf[16];
    if (m == MOVE_NONE) {
        snprintf(buf, sizeof(buf), "no hint");
    } else {
        snprintf(buf, sizeof(buf), "%s", move_to_str(m));
    }

    char *out = malloc(strlen(buf) + 1);
    if (!out) return NULL;
    strcpy(out, buf);
    return out;
}

typedef struct {
    Cube cube_copy;
} HintThreadArg;

static void *hint_thread_main(void *arg) {
    HintThreadArg *h = (HintThreadArg *)arg;
    char *hint = compute_hint_string(&h->cube_copy);
    if (hint) {
        printf("HINT: %s\n", hint);
        free(hint);
    }
    free(h);
    return NULL;
}

void start_hint_thread(const Cube *cube) {
    if (!cube) return;

    HintThreadArg *arg = malloc(sizeof(HintThreadArg));
    if (!arg) return;
    arg->cube_copy = *cube;

    pthread_t tid;
    if (pthread_create(&tid, NULL, hint_thread_main, arg) == 0) {
        pthread_detach(tid);
    } else {
        free(arg);
    }
}
