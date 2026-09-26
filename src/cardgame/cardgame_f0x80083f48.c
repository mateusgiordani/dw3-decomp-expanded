/*
 * CARDGAME:0x80083f48 CARDGAME_F0x80083f48
 * 60 bytes at CARDGAME.PRO offset 0x1298 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80083f48
 *  Symbols     (none)
 *  Compare     60 bytes from 0x80083f48 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80083f48
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Adds `amount` to p1[0x4e0] when the byte at +0x584 of the previous slot (p1 +
 * (p1[0x575] - 1) * 8) equals `kind`.
 */

#include <stdint.h>

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S8(p, o)     (*(int8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))

void CARDGAME_F0x80083f48(int32_t p1, int32_t p2, int32_t kind, int32_t amount)
{
    int32_t d;

    d = (S8(p1, 0x575) - 1) * 8;
    if (U8(p1 + d, 0x584) == kind)
        S16(p1, 0x4e0) += amount;
}
