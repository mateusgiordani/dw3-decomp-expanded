/*
 * CARDGAME:0x8009e62c CARDGAME_F0x8009e62c
 * 60 bytes at CARDGAME.PRO offset 0x1b97c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009e62c
 *  Symbols     (none)
 *  Compare     60 bytes from 0x8009e62c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009e62c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Applies a pending request p1[0x49d] (block at p1+0x498, byte +5): when set,
 * fills the 15 flags at p1+0x49e with (request == 2) and clears the request.
 *
 * Matching note: the fill is written as an ascending loop; GCC reverses it into
 * PAL's count-down with a reduced pointer, while a descending source loop stays
 * unreduced.
 */

#include <stdint.h>

#define U8(p, o)     (*(uint8_t *)((p) + (o)))

void CARDGAME_F0x8009e62c(int32_t p1)
{
    int32_t c;
    int32_t v;
    int32_t i;

    c = p1 + 0x498;
    if (U8(c, 5) != 0) {
        v = U8(c, 5) == 2;
        for (i = 0; i < 15; i++)
            U8(p1 + i, 0x49e) = v;
        U8(c, 5) = 0;
    }
}
