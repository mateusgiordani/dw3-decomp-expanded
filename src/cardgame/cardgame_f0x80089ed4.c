/*
 * CARDGAME:0x80089ed4 CARDGAME_F0x80089ed4
 * 68 bytes at CARDGAME.PRO offset 0x7224 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80089ed4
 *  Symbols     (none)
 *  Compare     68 bytes from 0x80089ed4 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80089ed4
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Returns the height for the larger of the two hand counts (p1[0x72c],
 * p1[0x79e]), at least 3 cards: n * 8 + 14.
 */

#include <stdint.h>

#define U8(p, o)     (*(uint8_t *)((p) + (o)))

int32_t CARDGAME_F0x80089ed4(int32_t p1)
{
    int32_t n;

    n = U8(p1, 0x72c) > U8(p1, 0x79e) ? U8(p1, 0x72c) : U8(p1, 0x79e);
    if (n < 3)
        n = 3;
    return n * 8 + 14;
}
