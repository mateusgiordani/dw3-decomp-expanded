// CARDGAME:0x80096a94 (size 396, 0x18c)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x13de4
// Framed function from boundary sweep reports/handoffs/cardgame-boundary-sweep.md #113
// Prologue 27bdff30 addiu sp,-0xd0 ; epilogue jr ra / 27bd00d0 addiu sp,+0xd0
// Prev CARDGAME:0x80096950 (324B) ends at 0x80096a94; next CARDGAME:0x80096c20 at +0x18c.
// Callers (Ghidra CARDGAME read-only x-ref to): 9 UNCONDITIONAL_CALL sites in
//   CARDGAME_F0x8009b168 (0x8009b240/0x8009b2b8), CARDGAME_F0x80096e8c (0x80096f98),
//   CARDGAME_F0x80098898 (0x80098e40/0x80098e7c/0x80098eb8/0x80098ef4/0x80098f44/0x80098fac).
// Callee: EXE F0x8001f648 (C_MATCHING, decomp/src/exe/exe_f0x8001f648.c) fills a
//   0xa0-byte callback struct at sp+0x10; this function then issues 5 indirect
//   calls through its slots (+0x74/+0x7c/+0x84/+0x8c/+0x94 struct-relative).
// Data: EXE function-pointer word at 0x80044f5c, reached in PAL as
//   lui s5,0x8004 / addiu s5,0x4b38 / lw v0,0x424(s5) with the lui/addiu hoisted
//   above the digit loop; expressed here via an EXE base local so the pinned
//   psyq-gcc-2.8.1 -O2 hoist reproduces the PAL schedule. Digit rendering via
//   signed /10 and %10 (magic 0x66666667, mult/mfhi/sra/subu).
// Ghidra CARDGAME disasm/decompile/xrefs read-only (project ddw3-pal-sles-03936,
// base 0x80082cb0, no import/mutation); upstream cardgame.s GUIDE only.
// Toolchain: PsyQ GCC 2.8.1 + ASPSX 2.79, o2-g0-no-strength-reduce.
#include <stdint.h>

extern void F0x8001f648(void *buf);
extern uint8_t D0x80044b38[];

typedef void (*CardCb2)(int32_t a0, int32_t a1);
typedef void (*CardCb3)(int32_t a0, int32_t a1, int32_t a2);
typedef void (*CardCb4)(int32_t a0, int32_t a1, int32_t a2, int32_t a3);
typedef int32_t (*ExeSlotFn)(int32_t a0);

typedef struct {
    uint8_t pad[0x74];
    CardCb2 cb74;
    CardCb2 cb78;
    CardCb2 cb7c;
    CardCb2 cb80;
    CardCb4 cb84;
    CardCb2 cb88;
    CardCb3 cb8c;
    CardCb2 cb90;
    CardCb2 cb94;
    CardCb2 cb98;
    CardCb2 cb9c;
} CardCtx96a94;

void CARDGAME_F0x80096a94(int16_t *a0, int32_t a1)
{
    CardCtx96a94 ctx;
    uint8_t *exebase;
    int32_t val;
    int32_t i;
    int32_t rem;
    int32_t k;
    int32_t r;

    F0x8001f648(&ctx);
    ctx.cb7c(0x100, ((uint8_t *)a0)[0x10]);
    ctx.cb74(0x340, 0);
    if (a1 != 0) {
        ctx.cb94(a0[5], a0[6]);
        ctx.cb8c(a0[3], a0[4], 0x1000);
    }
    val = a0[0];
    i = 0;
    if (((uint8_t *)a0)[0x0e] != 0) {
        do {
            rem = val % 10;
            if (i == 0 || ((uint8_t *)a0)[0x0f] != 0 || rem != 0 || val / 10 != 0) {
                exebase = D0x80044b38;
                r = ((ExeSlotFn)*(void **)(exebase + 0x424))(0x25d0003);
                k = i + 1;
                ctx.cb84(r, rem + 0x14, a0[1] + (((uint8_t *)a0)[0x0e] - k) * 7, a0[2]);
            }
            i += 1;
            val /= 10;
        } while (i < ((uint8_t *)a0)[0x0e]);
    }
}
