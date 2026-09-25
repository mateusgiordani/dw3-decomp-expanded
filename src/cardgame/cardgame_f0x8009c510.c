#include "common/types.h"

extern void (*DAT_8004B600)(void *, int32_t);

int CARDGAME_F0x8009c510(uint8_t *a0, int32_t a1, int32_t a2, int32_t a3) {
    void (*fn)(void *, int32_t);
    int32_t off;
    int32_t off2;
    uint8_t *elem;
    fn = DAT_8004B600;
    off = a1 * 76;
    off2 = off + 0x108;
    fn(a0 + off2, 76);
    elem = a0 + off;
    elem[0x14d] = 1;
    elem[0x14a] = 1;
    *(int32_t *)(elem + 0x108) = a2;
    *(int32_t *)(elem + 0x10c) = a3;
    *(int16_t *)(elem + 0x124) = 0x1000;
    *(int16_t *)(elem + 0x120) = 0x1000;
    *(int16_t *)(elem + 0x126) = 0x1000;
    *(int16_t *)(elem + 0x122) = 0x1000;
    *(int16_t *)(elem + 0x142) = 0;
    *(int16_t *)(elem + 0x140) = 0;
    *(int16_t *)(elem + 0x12c) = (int16_t)a1;
    elem[0x14b] = 0;
    elem[0x14c] = 0;
    elem[0x14f] = 0;
    elem[0x151] = 0;
    *(int16_t *)(elem + 0x12e) = 0;
    return 0;
}
