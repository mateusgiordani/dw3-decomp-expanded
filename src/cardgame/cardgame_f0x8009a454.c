// CARDGAME:0x8009a454 (size 236, 0xEC)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x177A4
// Ghidra CARDGAME read-only (project ddw3-pal-sles-03936):
//   disasm 59 words match PAL LE 1:1; decompile computes
//   *ctx = ctx[4] + table[(ctx[10] & 7)], helper((ctx[10] << 12) / 0x18),
//   half(ctx+0x1a) = (helper >> 4) + 0x1000, ctx[10] += (*D_8004DF9C)(),
//   threshold 12 sets flag/zeroes fields, returns flag.
// Xrefs: 1 caller CARDGAME_F0x8009b890 via 0x8009b97c (case 7, return ignored);
//   callees: EXE F0x8002abcc (direct jal) + indirect via D_8004DF9C.
// Upstream cardgame.s GUIDE only (submodule absent in worktree).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base).
#include "common/types.h"

extern int32_t EXE_F0x8002abcc(int32_t x);
extern int32_t (*D_8004DF9C)(void);
extern int16_t D_800A5AC8[];

int32_t CARDGAME_F0x8009a454(int32_t a0, int32_t *a1)
{
    int32_t tmp;
    int32_t flag;

    (void)a0;
    flag = 0;
    a1[0] = a1[4] + D_800A5AC8[a1[10] & 7];
    tmp = EXE_F0x8002abcc((a1[10] << 12) / 0x18);
    if (tmp < 0)
        tmp += 15;
    *(int16_t *)((char *)a1 + 0x1a) = (int16_t)(tmp >> 4) + 0x1000;
    a1[10] += D_8004DF9C();
    if (a1[10] >= 12) {
        /* Store-then-set: scheduler CSEs constant 1 with flag (move v0,s1;
           sb v0) and delays the 0x1000 materialization; flag-first form
           hoists li v0,0x1000 above the store (-1 word). */
        *(char *)((char *)a1 + 0x42) = 1;
        flag = 1;
        *(int16_t *)((char *)a1 + 0x26) = 0;
        *(char *)((char *)a1 + 0x47) = 0;
        *(int16_t *)((char *)a1 + 0x1a) = 0x1000;
        a1[0] = a1[4];
    }
    return flag;
}
