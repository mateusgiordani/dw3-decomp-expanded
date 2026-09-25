#include "common/types.h"

extern int32_t (*D_8004DF98)(void);

void STCRDSHP_func_80085cd8(uint8_t *self) {
    int32_t mode;
    int32_t now;

    mode = *(int32_t *)(self + 0x10);
    if (mode == 1) {
        goto mode1;
    }
    if (1 < mode) {
        if (mode == 2) {
            goto mode2;
        }
    }
    return;
mode1:
    if (*(int32_t *)(self + 0x64) != 0) {
        *(int32_t *)(self + 0x64) -= 1;
        ((void (*)(uint8_t *))(*(void (**)(void))(self + 0x3c)))(self);
        *(int32_t *)(self + 0x18) = D_8004DF98();
    } else {
        *(int32_t *)(self + 0x0c) = 3;
    }
    return;
mode2:
    now = D_8004DF98();
    if (1 < now - *(int32_t *)(self + 0x18)) {
        *(int32_t *)(self + 0x10) = 1;
    }
}
