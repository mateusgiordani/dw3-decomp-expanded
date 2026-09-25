#include "common/types.h"

/* STCRDABM:0x80082d88 (136 bytes, 34 instructions); PAL reviewed body. */
void STCRDABM_func_80082d88(int32_t object, int32_t direction, int32_t divisor) {
    ((void (*)(int32_t, int32_t))*(void **)(object + 0x28))(object, 1);
    *(int32_t *)(object + 0x10) = 1;
    *(int32_t *)(object + 0x58) = direction;
    if (direction == 0) {
        *(int32_t *)(object + 0x5c) = 0;
        *(int32_t *)(object + 0x60) = 0xff00 / divisor;
    } else {
        *(int32_t *)(object + 0x5c) = 0xff00;
        *(int32_t *)(object + 0x60) = -(0xff00 / divisor);
    }
}
