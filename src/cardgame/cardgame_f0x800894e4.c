#include "common/types.h"

/* CARDGAME:0x800894e4 (size 156, 0x9c)
 * PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x6834
 * Framed -0x30/+0x30; next framed function CARDGAME:0x80089580 at +0x9c.
 * 7 direct jal callers at 0x80084a70..0x80084b00 (a2 = 0..6, dispatch fan-in);
 * 0 direct callees (2 indirect jalr via slots 0xEE4/0xEAC on second arg).
 * D_800A58C4 is a 2-byte-entry table indexed by idx (callers pass 0..6).
 * Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0, exact_byte_match.
 */

typedef void (*CardFn5)(void *a0, uint32_t a1, int32_t a2, int32_t a3, int32_t a4);
typedef void (*CardFn6)(void *a0, int32_t a1, int32_t a2, uint32_t a3, int32_t a4, int32_t a5);

extern uint8_t D_800A58C4[];

void CARDGAME_F0x800894e4(uint8_t *ctx, uint8_t *s, int32_t idx) {
    uint8_t *p = &D_800A58C4[idx * 2];
    int32_t one = 1;

    ((CardFn5)*(uint32_t *)(s + 0xEE4))(s, p[1], 0, 0, one);
    ((CardFn6)*(uint32_t *)(s + 0xEAC))(s, 5, 5, p[0], 0, 66);
    ctx[0x422] = (uint8_t)one;
}
