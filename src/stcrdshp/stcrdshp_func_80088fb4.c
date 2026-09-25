#include "common/types.h"

extern void (*D_80055C48)(int32_t);

void STCRDSHP_func_80088fb4(int32_t *state, int32_t reverse) {
    state[3] = 1;
    /* Branch-local results avoid the shared GCC pseudo's v0/v1 conflicts. */
    if (reverse != 0) {
        int32_t step;
        D_80055C48(0x40019);
        step = 0x1000 / state[0];
        state[2] = 0;
        state[1] = step;
    } else {
        int32_t origin;
        int32_t step;
        D_80055C48(0x4001a);
        origin = 0x1000;
        state[2] = origin;
        step = (origin / state[0]) * -2;
        state[1] = step;
    }
}
