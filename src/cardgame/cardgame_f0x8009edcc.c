/*
 * CARDGAME:0x8009edcc CARDGAME_F0x8009edcc
 * 304 bytes at CARDGAME.PRO offset 0x1c11c (overlay loaded at 0x80082cb0).
 *
 * Byte-match recipe (generated from recipes/card_cage.json by
 * tools/recipe_headers.py). Compiling this file as below reproduces the PAL
 * bytes of the function.
 *
 *  Preprocess  clang -E -nostdinc -include include/ps1_types.h -I include
 *  Compile     cc1 -quiet -O2 -G0 -mips1 -msoft-float -fno-strength-reduce
 *  Variant     o2-g0-no-strength-reduce
 *  Toolchain A (public, default)
 *    cc1       gcc-2.8.1-psx (decompals/old-gcc)
 *    assemble  maspsx 874855c --aspsx-version=2.79, then mipsel-linux-gnu-as
 *              -EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0
 *  Toolchain B (original PsyQ, optional)
 *    cc1       CC1PSX 2.8.1 SN32 BUILD 4.0.0010
 *    assemble  ASPSX 2.79, after removing the zero-divisor guard
 *              (tools/div_guard.py); the overlays have none and ASPSX would
 *              insert one after every division.
 *  Link        .text at 0x8009edcc
 *  Symbols     CARDGAME_F0x8009e668=0x8009e668
 *  Compare     304 bytes from 0x8009edcc against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009edcc
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * PAL @ 0x80082cb0 base
 *
 * PAL bytes verified word-for-word (prologue d0ffbd27 .. epilogue j ra). Caller
 * via CARDGAME_F0x8009e668 etc.
 *
 * Semantics: double loop (outer 2 x inner 5) + 5 tail calls per outer, all via
 * *(a1+0x18)+0xea0 slot. a0 = uint8_t *base (stride 0xc8), a1 = context with
 * slot at +0x18. Inner byte at base+off+inner+0x5a8 (lbu), outer halfwords at
 * base+off+0x5a4/0x5a6/0x5a2/0x59c/0x59e (lh). Outer offset off = outer*0xc8,
 * cur = a0+off (both kept in s-regs per PAL: s2=cur, s3=off, s5=base).
 *
 * Codegen notes (psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79, -O2 -G0): Base
 * variant GIV-folds cur into base+0x59e induction (lh 6($18) etc) = 29B diff.
 * o2-g0-no-strength-reduce (-fno-strength-reduce) keeps cur in s2 with large
 * immediates (lh 0x5a4(s2) etc); inner order forced via a0+(inner+off)
 * spelling. The idx5 tail block keeps off += 0xc8 after the callback; this
 * placement produces PAL's li a2,5 before the loads and addiu s3,0xc8 in the
 * jalr delay slot. The canonical o2-g0-no-strength-reduce build is
 * byte-identical across all 304 bytes: frame 0x30, spill order, regs
 * (s5=base,s4=ctx,s1=outer,s0=inner,s2=cur,s3=off), inner two-add, 4/5 tail
 * blocks, epilogue.
 */

#include <stdint.h>

typedef struct {
    uint8_t pad[0x59c];
    int16_t f59c; // 0x59c
    int16_t f59e; // 0x59e
    uint8_t gap[2]; // 0x5a0-0x5a1
    int16_t f5a2; // 0x5a2
    int16_t f5a4; // 0x5a4
    int16_t f5a6; // 0x5a6
    uint8_t f5a8; // 0x5a8
} S;

void CARDGAME_F0x8009edcc(uint8_t *a0, uint8_t *a1) {
    int32_t outer = 0;
    int32_t inner = 0;
    uint8_t *cur = a0;
    int32_t off = 0;
    do {
        inner = 0;
        do {
            int32_t ctx = *(int32_t *)(a1 + 0x18);
            int32_t fn = *(int32_t *)(ctx + 0xea0);
            ((void (*)(int32_t, int32_t, int32_t, int32_t))fn)(ctx, outer, inner, ((uint8_t *)(a0 + (inner + off)))[0x5a8]);
            inner++;
        } while (inner < 5);
        {
            int32_t ctx = *(int32_t *)(a1 + 0x18);
            int32_t fn = *(int32_t *)(ctx + 0xea0);
            ((void (*)(int32_t, int32_t, int32_t, int32_t))fn)(ctx, outer, 5, ((S *)cur)->f5a4);
            off += 0xc8;
        }
        {
            int32_t ctx = *(int32_t *)(a1 + 0x18);
            int32_t fn = *(int32_t *)(ctx + 0xea0);
            ((void(*)(int32_t,int32_t,int32_t,int32_t))fn)(ctx, outer, 6, ((S*)cur)->f5a6);
        }
        {
            int32_t ctx = *(int32_t *)(a1+0x18);
            int32_t fn = *(int32_t*)(ctx+0xea0);
            ((void(*)(int32_t,int32_t,int32_t,int32_t))fn)(ctx, outer, 7, ((S*)cur)->f5a2);
        }
        {
            int32_t ctx = *(int32_t*)(a1+0x18);
            int32_t fn = *(int32_t*)(ctx+0xea0);
            ((void(*)(int32_t,int32_t,int32_t,int32_t))fn)(ctx, outer, 8, ((S*)cur)->f59c);
        }
        {
            int32_t ctx = *(int32_t*)(a1+0x18);
            int32_t fn = *(int32_t*)(ctx+0xea0);
            ((void(*)(int32_t,int32_t,int32_t,int32_t))fn)(ctx, outer, 9, ((S*)cur)->f59e);
        }
        cur += 0xc8;
        outer++;
    } while (outer < 2);
}
