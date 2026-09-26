/*
 * CARDGAME:0x8008404c CARDGAME_F0x8008404c
 * 68 bytes at CARDGAME.PRO offset 0x139c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008404c
 *  Symbols     (none)
 *  Compare     68 bytes from 0x8008404c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008404c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Adds `amount` to p1[0x4e0] when the side holds fewer than 6 cards (count p1 +
 * side * 0x72 + 0x72c).
 */

#include <stdint.h>

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))

void CARDGAME_F0x8008404c(int32_t p1, int32_t side, int32_t amount)
{
    int32_t o;

    o = side * 0x72;
    if (U8(p1 + o, 0x72c) < 6)
        S16(p1, 0x4e0) += amount;
}
