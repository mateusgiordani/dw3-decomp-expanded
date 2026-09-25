#include "common/types.h"

/* STCRDABM:0x80083e14 (664 bytes, 0x80083e14..0x800840ac)
 * PAL-SLES-03936; verified base 0x80082cb0, reviewed disasm+decompiler.
 * Seven slot objects refreshed from context counters at +0x60/+0x64/+0x68.
 * flag == 0 hides all seven slots (method +0x144 with 0); otherwise each
 * slot is configured through methods +0x114/+0x118/+0x148, sharing one
 * group value from D_80044B38.fn414(D_8005CCA8 + 0x24).
 */
extern struct {
    char pad[0x414];
    int32_t (*fn414)(int32_t arg);
} D_80044B38;
extern int32_t D_8005CCA8;

void STCRDABM_func_80083e14(int32_t ctx, int32_t *slots, int32_t flag) {
    if (flag != 0) {
        ((void (*)(int32_t, int32_t, int32_t))*(int32_t *)(slots[0] + 0x114))(slots[0], D_80044B38.fn414(D_8005CCA8 + 0x24), 1);
        ((void (*)(int32_t, int32_t, int32_t))*(int32_t *)(slots[1] + 0x114))(slots[1], D_80044B38.fn414(D_8005CCA8 + 0x24), 2);
        ((void (*)(int32_t, int32_t, int32_t))*(int32_t *)(slots[2] + 0x118))(slots[2], 0, *(int32_t *)(ctx + 0x60) + 1);
        ((void (*)(int32_t, int32_t))*(int32_t *)(slots[2] + 0x148))(slots[2], 1);
        ((void (*)(int32_t, int32_t, int32_t))*(int32_t *)(slots[3] + 0x114))(slots[3], D_80044B38.fn414(D_8005CCA8 + 0x24), 5);
        ((void (*)(int32_t, int32_t, int32_t))*(int32_t *)(slots[4] + 0x118))(slots[4], 0, *(int32_t *)(ctx + 0x64));
        ((void (*)(int32_t, int32_t))*(int32_t *)(slots[4] + 0x148))(slots[4], 1);
        if (*(int32_t *)(ctx + 0x68) == 0) {
            return;
        }
        if (*(int32_t *)(ctx + 0x60) > 0) {
            ((void (*)(int32_t, int32_t, int32_t))*(int32_t *)(slots[5] + 0x114))(slots[5], D_80044B38.fn414(D_8005CCA8 + 0x24), 3);
        } else {
            ((void (*)(int32_t, int32_t))*(int32_t *)(slots[5] + 0x144))(slots[5], 0);
        }
        if (*(int32_t *)(ctx + 0x60) < *(int32_t *)(ctx + 0x64) - 1) {
            ((void (*)(int32_t, int32_t, int32_t))*(int32_t *)(slots[6] + 0x114))(slots[6], D_80044B38.fn414(D_8005CCA8 + 0x24), 4);
            return;
        }
    } else {
        ((void (*)(int32_t, int32_t))*(int32_t *)(slots[0] + 0x144))(slots[0], 0);
        ((void (*)(int32_t, int32_t))*(int32_t *)(slots[1] + 0x144))(slots[1], 0);
        ((void (*)(int32_t, int32_t))*(int32_t *)(slots[2] + 0x144))(slots[2], 0);
        ((void (*)(int32_t, int32_t))*(int32_t *)(slots[3] + 0x144))(slots[3], 0);
        ((void (*)(int32_t, int32_t))*(int32_t *)(slots[4] + 0x144))(slots[4], 0);
        ((void (*)(int32_t, int32_t))*(int32_t *)(slots[5] + 0x144))(slots[5], 0);
    }
    ((void (*)(int32_t, int32_t))*(int32_t *)(slots[6] + 0x144))(slots[6], 0);
}
