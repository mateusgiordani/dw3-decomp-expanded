// CARDGAME:0x8009c9a0 (size 156, 0x9c)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x19cf0
// Framed function from boundary sweep reports/handoffs/cardgame-boundary-sweep.md #169
// Prologue 27bdffd0 addiu sp,-0x30 ; epilogue 27bd0030 addiu sp,+0x30 at jr ra+4
// Next framed CARDGAME:0x8009ca3c at +0x9c confirms size 0x9c contiguous;
// prev CARDGAME:0x8009c938 size 0x28 then leaf gap 0x8009c960 (64B) to here.
// Ghidra program CARDGAME (base 0x80082cb0) has 0 analyzed functions at this
// address (expected per sweep); verification uses direct PAL word decode.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base).

#include "common/types.h"

extern void (*D_80055c48)(uint32_t);
extern void CARDGAME_F0x8009c960(void *a0, int32_t a1, int32_t a2, int32_t a3, int32_t a4);

void CARDGAME_F0x8009c9a0(void *a0, int32_t a1, int32_t a2, int32_t a3, int32_t a4)
{
    char *p;

    D_80055c48(0x8004603c);
    p = (char *)a0 + a1 * 76;
    p[330] = 2;
    CARDGAME_F0x8009c960(a0, a1, a2, a3, a4);
}
