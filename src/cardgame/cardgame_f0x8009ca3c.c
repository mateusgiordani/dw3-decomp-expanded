// CARDGAME:0x8009ca3c (size 68, 0x44)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x19d8c
// Framed function from boundary sweep reports/handoffs/cardgame-boundary-sweep.md #169
// Prologue 27bdffe0 addiu sp,-0x20 ; jal 0x8009c960 ; epilogue 27bd0020
// Next framed CARDGAME:0x8009cda4 at +0x324 gap (leaf code), prev CARDGAME:0x8009c9a0 at -0x9c confirms size 0x44 contiguous.

#include "common/types.h"

extern void CARDGAME_F0x8009c960(void *a0, int32_t a1, int32_t a2, int32_t a3, int32_t a4);

void CARDGAME_F0x8009ca3c(void *a0, int32_t a1, int32_t a2, int32_t a3, int32_t a4)
{
    char *p = (char *)a0 + a1 * 76;
    p[330] = 3;
    CARDGAME_F0x8009c960(a0, a1, a2, a3, a4);
}
