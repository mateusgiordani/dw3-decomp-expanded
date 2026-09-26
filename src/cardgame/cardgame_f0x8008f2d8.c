/*
 * CARDGAME:0x8008f2d8 CARDGAME_F0x8008f2d8
 * 20 bytes at CARDGAME.PRO offset 0xc628 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008f2d8
 *  Symbols     (none)
 *  Compare     20 bytes from 0x8008f2d8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008f2d8
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Stores b at p1[0x430] and a at p1[0x434] and sets p1[0x423].
 */

#include <stdint.h>

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S32(p, o)    (*(int32_t *)((p) + (o)))

void CARDGAME_F0x8008f2d8(int32_t p1, int32_t p2, int32_t a, int32_t b)
{
    S32(p1, 0x430) = b;
    S32(p1, 0x434) = a;
    U8(p1, 0x423) = 1;
}
