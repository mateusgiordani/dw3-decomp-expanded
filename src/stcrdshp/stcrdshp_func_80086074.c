#include "common/types.h"

extern int32_t EXE_F0x8001ae38(int32_t, int32_t, int32_t, int32_t);
extern int32_t EXE_F0x8001b364(int32_t, int32_t, int32_t, int32_t);

void STCRDSHP_func_80086074(void *self, int32_t *out) {

    int32_t object;

#define LOAD(slot, kind, x, y) out[slot] = EXE_F0x8001ae38(*(int16_t *)((uint8_t *)self + 0x54), kind, x, y)
    LOAD(0, 1, 0x88, 0x80);
    LOAD(1, 1, 0x115, 0x80);
    LOAD(2, 1, 0x126, 0x80);
    LOAD(3, 1, 0x115, 0xa6);
    LOAD(4, 1, 300, 0xa6);
    LOAD(5, 1, 0x50, 0x97);
    LOAD(6, 1, 0xce, 0x97);
    LOAD(7, 1, 0xf0, 0x97);
    LOAD(8, 1, 0xce, 0xa4);
    LOAD(9, 1, 0xf0, 0xa4);
    LOAD(11, 3, 0x117, 199);
    LOAD(10, 3, 0x11a, 199);
    LOAD(12, 1, 0x12, 0x67);
    LOAD(13, 1, 0x121, 0x67);
    object = out[13];
    ((void (*)(int32_t *, int32_t))( *(void (**)(void))(object + 0x15c) ))((int32_t *)object, *(int32_t *)((uint8_t *)self + 0x58));
    LOAD(14, 1, 0x9a, 0x39);
    LOAD(15, 1, 0xc5, 0x56);
    LOAD(16, 1, 0xc5, 0x66);
    object = EXE_F0x8001b364(*(int16_t *)((uint8_t *)self + 0x54), *(int32_t *)((uint8_t *)self + 0x58) - 3, 0xb8, 0x56);
    out[17] = object;
    ((void (*)(int32_t *, int32_t))( *(void (**)(void))(object + 0x80) ))((int32_t *)object, 0);
#undef LOAD
}
