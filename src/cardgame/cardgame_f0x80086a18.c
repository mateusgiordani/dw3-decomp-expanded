/*
 * CARDGAME:0x80086a18 CARDGAME_F0x80086a18
 * 24 bytes at CARDGAME.PRO offset 0x3d68 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80086a18
 *  Symbols     (none)
 *  Compare     24 bytes from 0x80086a18 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80086a18
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Clears the halfword at p2+0xe32 and the words at p2+0xe34, +0xe4c, +0xe64 and
 * +0xe7c.
 */

#include <stdint.h>

#define S16(p, o)    (*(int16_t *)((p) + (o)))
#define S32(p, o)    (*(int32_t *)((p) + (o)))

void CARDGAME_F0x80086a18(int32_t p1, int32_t p2)
{
    S16(p2, 0xe32) = 0;
    S32(p2, 0xe34) = 0;
    S32(p2, 0xe4c) = 0;
    S32(p2, 0xe64) = 0;
    S32(p2, 0xe7c) = 0;
}
