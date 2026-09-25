#include <stdint.h>

extern void F0x8001ebf8(void *buf);

void CARDGAME_F0x800a0628(uint8_t *a0, int32_t a1, int32_t a2, uint32_t a3)
{
    struct {
        uint8_t *p;
        int32_t w[10];
        void (*fn)(int32_t);
        int32_t tail[10];
    } tmp;
    int32_t addr;
    int32_t t;
    uint8_t *entry;
    uint8_t *group;
    uint8_t cnt;

    addr = a1 * 0x72 + 0x72c;
    group = a0 + addr;
    F0x8001ebf8(&tmp);
    t = a2 * 2;
    tmp.fn((int32_t)(*(int16_t *)(a0 + t + 0x50) + 1));
    entry = group + a3 * 0xe;
    *(uint16_t *)(entry + 8) = tmp.p[1];
    *(uint16_t *)(entry + 10) = tmp.p[2];
    *(uint16_t *)(entry + 4) = 0;
    *(uint16_t *)(entry + 6) = 0;
    *(int16_t *)(entry + 2) = (int16_t)a2;
    entry[13] = (uint8_t)a1;
    entry[12] = (uint8_t)a1;
    cnt = a0[0x308];
    a0[0x308] = (uint8_t)(cnt + 1);
    entry[14] = cnt;
}
