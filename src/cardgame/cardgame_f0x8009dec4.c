#include "common/types.h"

int32_t CARDGAME_F0x8009dec4(uint8_t *a0, uint8_t *a1) {
    int32_t s1 = 0;
    uint8_t *s0 = a0;
    uint8_t *s2 = a1;
    if (*(int32_t *)(s0 + 0x4e8) != 0) goto L1;
    *(uint8_t *)(s0 + 0x499) = 2;
    {
        /* Slot writer 0x8009d4c0 installs 0x8009c350, which accesses a0. */
        uint8_t *obj = *(uint8_t **)(s2 + 0x18);
        (*(void (**)(uint8_t *))(obj + 0xec4))(obj);
    }
    *(int32_t *)(s0 + 0x4e8) = *(int32_t *)(s0 + 0x4e8) + 1;
L1:
    if (*(int16_t *)(*(int32_t *)(s2 + 0x18) + 0x64) != 0) goto L2;
    if (*(uint8_t *)(s0 + 0x498) != 0) goto L2;
    s1 = 1;
L2:
    return s1;
}
