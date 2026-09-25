#include "common/types.h"

extern int32_t (*D0x80055c48)(int32_t);

/* STCRDABM:0x800858a0 (148 bytes, 37 instructions); PAL reviewed body. */
void STCRDABM_func_800858a0(int32_t *object, int32_t direction) {
    object[3] = 1;
    /* Branch-local results avoid the shared GCC pseudo's v0/v1 conflicts. */
    if (direction != 0) {
        int32_t step;
        D0x80055c48(0x40019);
        step = 0x1000 / object[0];
        object[2] = 0;
        object[1] = step;
    } else {
        int32_t origin;
        int32_t step;
        D0x80055c48(0x4001a);
        origin = 0x1000;
        object[2] = origin;
        step = (origin / object[0]) * -2;
        object[1] = step;
    }
}
