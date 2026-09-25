#include "common/types.h"

extern void STCRDABM_func_80082e10(int32_t object);

/* STCRDABM:0x80082f54 (180 bytes, 45 instructions); PAL reviewed body. */
void STCRDABM_func_80082f54(int32_t object) {
    int32_t status;
    int32_t value;

    status = *(int32_t *)(object + 0xc);
    if (status != 1) {
        if (status > 1) {
            if (status == 2) {
                goto update;
            }
            if (status == 3) {
                return;
            }
        }
        ((void (*)(int32_t))*(void **)(object + 0x38))(object);
        return;
    }
    if (*(int32_t *)(object + 0x10) == 0) {
        return;
    }
    value = *(int32_t *)(object + 0x5c) + *(int32_t *)(object + 0x60);
    *(int32_t *)(object + 0x5c) = value;
    if (*(int32_t *)(object + 0x58) == 0) {
        if (value < 0xff01) {
            goto update;
        }
        *(int32_t *)(object + 0x5c) = 0xff00;
        *(int32_t *)(object + 0xc) = 2;
        goto update;
    }
    if (value >= 0) {
        goto update;
    }
    *(int32_t *)(object + 0x5c) = 0;
    *(int32_t *)(object + 0xc) = 2;
update:
    STCRDABM_func_80082e10(object);
}
