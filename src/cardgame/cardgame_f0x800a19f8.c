/*
 * CARDGAME:0x800a19f8 CARDGAME_F0x800a19f8
 * 100 bytes at CARDGAME.PRO offset 0x1ed48 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a19f8
 *  Symbols     (none)
 *  Compare     100 bytes from 0x800a19f8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a19f8
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Moves every remaining entry of the int16 list at p+0x64 (count p+0xa, taken
 * from the end) onto the int16 stack at p+0x14 (top index p+4, filled
 * downwards), counting each move in p+8.
 *
 * Matching note: the pre-decremented indices feed named byte offsets a/b, so
 * the decremented values are reused and both adds are base-first as in PAL.
 */

#include <stdint.h>

#define S16(p, o)    (*(int16_t *)((p) + (o)))

void CARDGAME_F0x800a19f8(int32_t p)
{
    int32_t a;
    int32_t b;

    while (S16(p, 0xa) > 0) {
        a = --S16(p, 4) * 2;
        b = --S16(p, 0xa) * 2;
        S16(p + a, 0x14) = S16(p + b, 0x64);
        S16(p, 8)++;
    }
}
