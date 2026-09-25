#include "common/types.h"

void STCRDSHP_func_800853b4(void *self, int32_t direction, int32_t duration) {
    uint8_t *state = self;
    ((void (*)(void *, int32_t))(*(void (**)(void *))(state + 0x28)))(self, 1);
    *(int32_t *)(state + 0x10) = 1;
    *(int32_t *)(state + 0x58) = direction;
    if (direction == 0) {
        int32_t step;
        step = 0xff00 / duration;
        *(int32_t *)(state + 0x5c) = 0;
        *(int32_t *)(state + 0x60) = step;
    } else {
        int32_t step;
        step = -(0xff00 / duration);
        *(int32_t *)(state + 0x5c) = 0xff00;
        *(int32_t *)(state + 0x60) = step;
    }
}
