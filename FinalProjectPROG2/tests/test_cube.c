#include <stdio.h>
#include "../src/cube.h"

int main(void) {
    Cube c;

    /* initialize to solved state */
    cube_init_solved(&c);

    if (!cube_is_solved(&c)) {
        printf("test_cube: FAILED (cube not solved after init)\n");
        return 1;
    }

    /* apply some moves, it should NOT be solved anymore */
    cube_apply_algorithm(&c, "R U R'");

    if (cube_is_solved(&c)) {
        printf("test_cube: FAILED (cube still solved after moves)\n");
        return 1;
    }

    printf("test_cube: OK\n");
    return 0;
}
