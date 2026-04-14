#ifndef HINTS_H
#define HINTS_H

#include "cube.h"
#include "solver.h"

// Return a newly allocated string like "U", "R2", "no hint".
// Caller must free() the returned buffer.
char *compute_hint_string(const Cube *cube);

// Start a detached thread that prints a hint to stdout.
// This is where we actually "use threads" for your project.
void start_hint_thread(const Cube *cube);

#endif
