/* CARDGAME:0x80089f18, full 336-byte PAL match with GCC 2.8.1 / ASPSX 2.79 O2 G0.
 * Materialize each scaled index before forming the memory address: this keeps
 * base-first addu operand order with destination v0. See strategy-r7.md. */
#include <stdint.h>

typedef void (*cardgame_exe_vec_t)(uint32_t);

void CARDGAME_F0x80089f18(int32_t s, int32_t c, int32_t kind, int32_t delta) {
    int32_t off = 0;

    ((cardgame_exe_vec_t)*(uint32_t *)0x80055c48)(0x4001b);
    switch (kind) {
    case 1:
        off = 6;
        break;
    case 2:
        off = 12;
        break;
    }
    {
        int32_t k = (off + *(int32_t *)(s + 0x43c)) * 76;
        *(uint8_t *)(c + k + 0x150) &= 0xfe;
    }
    {
        int32_t k = (off + *(int32_t *)(s + 0x43c)) * 76;
        *(int16_t *)(c + k + 0x12e) = 0;
    }
    *(int32_t *)(s + 0x43c) += delta;
    {
        int32_t k = (off + *(int32_t *)(s + 0x43c)) * 76;
        *(uint8_t *)(c + k + 0x150) |= 1;
    }
    {
        int32_t k = (off + *(int32_t *)(s + 0x43c)) * 76;
        *(int16_t *)(c + k + 0x12e) = 1;
    }
}
