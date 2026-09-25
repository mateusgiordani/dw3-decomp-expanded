/*
 * CARDGAME:0x8008c988 CARDGAME_F0x8008c988
 * 196 bytes at CARDGAME.PRO offset 0x9cd8 (overlay loaded at 0x80082cb0).
 *
 * Byte-match recipe (generated from recipes/card_cage.json by
 * tools/recipe_headers.py). Compiling this file as below reproduces the PAL
 * bytes of the function.
 *
 *  Preprocess  clang -E -nostdinc -include include/ps1_types.h -I include
 *  Compile     cc1 -quiet -O2 -G0 -mips1 -msoft-float
 *  Variant     base
 *  Toolchain A (public, default)
 *    cc1       gcc-2.8.1-psx (decompals/old-gcc)
 *    assemble  maspsx 874855c --aspsx-version=2.79, then mipsel-linux-gnu-as
 *              -EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0
 *  Toolchain B (original PsyQ, optional)
 *    cc1       CC1PSX 2.8.1 SN32 BUILD 4.0.0010
 *    assemble  ASPSX 2.79, after removing the zero-divisor guard
 *              (tools/div_guard.py); the overlays have none and ASPSX would
 *              insert one after every division.
 *  Link        .text at 0x8008c988
 *  Symbols     CARDGAME_F0x8008c988=0x8008c988 F0x8001ebf8=0x8001ebf8
 *  Compare     196 bytes from 0x8008c988 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008c988
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Prologue: addiu sp,-0x80 ; sw s4,0x78(sp) ; move s4,a0 ; sw ra,0x7c(sp) ; sw
 * s3,0x74(sp) ; sw s2,0x70(sp) ; sw s1,0x6c(sp) ; sw s0,0x68(sp)
 *
 * Body: lh s3,0x5a0(s4) ; jal 0x8001ebf8 with a0=sp+0x10 (delay) ; lh
 * s0,0x5a0(s4) ; slti v0,s0,0x28 ; beqz -> store ; li s2,0x190 (delay) loop:
 * sll v0,s0,1 ; addu s1,v0,s4 ; lh v0,0x5b0(s1) ; sll v0,1 ; addu v0,s4,v0 ; lh
 * a0,0x50(v0) ; lw v0,0x3c(sp) ; jalr v0 with a0+1 (delay) lw v0,0x10(sp) ; lh
 * v1,8(v0) ; slt v0,(v1<s2) ; beqz -> next ; move s2,v1 + move s3,s0 ; next:
 * s0++ ; slti/bnez loop with s1+=2 (delay)
 *
 * Store: sw s3,0x440(s4) ; epilogue restores ra/s4/s3/s2/s1/s0 ; jr ra ; addiu
 * sp,+0x80
 *
 * Callee: EXE 0x8001ebf8 (DISASSEMBLED) fills struct at sp+0x10 (clears 0x54
 * bytes, installs fn-ptrs; +0x2c = indirect target, +0x0 -> base whose +8 holds
 * score)
 *
 * Ghidra: CARDGAME_F0x8008c988 decompile = min-search over s0 in
 * [*(s4+0x5a0),0x28), best index stored as word at s4+0x440; disasm words
 * verified equal to PAL LE words (49 words; capstone and ghidra-cli disasm
 * agree)
 */

#include "common/types.h"

extern void F0x8001ebf8(void *buf);

void CARDGAME_F0x8008c988(void *arg) {
    uint8_t buf[0x54];
    int32_t best;
    int32_t cur;
    int32_t limit;
    int16_t *tab;
    void (*fn)(int32_t);
    int16_t idx;
    char *row;
    int32_t val;
    int32_t score;

    best = *(int16_t *)((char *)arg + 0x5a0);
    F0x8001ebf8(buf);
    cur = *(int16_t *)((char *)arg + 0x5a0);
    limit = 0x190;
    if (cur < 0x28) {
        tab = (int16_t *)((cur << 1) + (int32_t)arg);
        do {
            idx = *(int16_t *)((char *)tab + 0x5b0);
            row = (char *)arg + (idx << 1);
            val = *(int16_t *)(row + 0x50);
            fn = *(void (**)(int32_t))(buf + 0x2c);
            fn(val + 1);
            score = *(int16_t *)(*(char **)buf + 8);
            if (score < limit) {
                limit = score;
                best = cur;
            }
            cur += 1;
            tab += 1;
        } while (cur < 0x28);
    }
    *(int32_t *)((char *)arg + 0x440) = best;
}
