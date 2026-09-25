/*
 * CARDGAME:0x8009c9a0 CARDGAME_F0x8009c9a0
 * 156 bytes at CARDGAME.PRO offset 0x19cf0 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009c9a0
 *  Symbols     CARDGAME_F0x8009c960=0x8009c960 D_80055c48=0x80055c48
 *  Compare     156 bytes from 0x8009c9a0 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009c9a0
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Prologue 27bdffd0 addiu sp,-0x30 ; epilogue 27bd0030 addiu sp,+0x30 at jr
 * ra+4
 *
 * Next framed CARDGAME:0x8009ca3c at +0x9c confirms size 0x9c contiguous; prev
 * CARDGAME:0x8009c938 size 0x28 then leaf gap 0x8009c960 (64B) to here.
 *
 * Ghidra program CARDGAME (base 0x80082cb0) has 0 analyzed functions at this
 * address (expected per sweep); verification uses direct PAL word decode.
 */

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
