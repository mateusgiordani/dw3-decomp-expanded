/*
 * CARDGAME:0x8009ca3c CARDGAME_F0x8009ca3c
 * 68 bytes at CARDGAME.PRO offset 0x19d8c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009ca3c
 *  Symbols     CARDGAME_F0x8009c960=0x8009c960
 *  Compare     68 bytes from 0x8009ca3c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009ca3c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Prologue 27bdffe0 addiu sp,-0x20 ; jal 0x8009c960 ; epilogue 27bd0020
 *
 * Next framed CARDGAME:0x8009cda4 at +0x324 gap (leaf code), prev
 * CARDGAME:0x8009c9a0 at -0x9c confirms size 0x44 contiguous.
 */

#include "common/types.h"

extern void CARDGAME_F0x8009c960(void *a0, int32_t a1, int32_t a2, int32_t a3, int32_t a4);

void CARDGAME_F0x8009ca3c(void *a0, int32_t a1, int32_t a2, int32_t a3, int32_t a4)
{
    char *p = (char *)a0 + a1 * 76;
    p[330] = 3;
    CARDGAME_F0x8009c960(a0, a1, a2, a3, a4);
}
