/*
 * CARDGAME:0x8008f0b4 CARDGAME_F0x8008f0b4
 * 64 bytes at CARDGAME.PRO offset 0xc404 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008f0b4
 *  Symbols     (none)
 *  Compare     64 bytes from 0x8008f0b4 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008f0b4
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Starts a count for `side`: sets p1[0x422], clears p1[0x424] and p1[0x430] and
 * copies the side record's (p1 + side * 0xc8) values at +0x5a6 and +0x5a2 into
 * p1[0x428] and p1[0x42c].
 */

#include <stdint.h>

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))
#define S32(p, o)    (*(int32_t *)((p) + (o)))

void CARDGAME_F0x8008f0b4(int32_t p1, int32_t p2, int32_t side)
{
    int32_t me;

    U8(p1, 0x422) = 1;
    me = p1 + side * 0xc8;
    S32(p1, 0x424) = 0;
    S32(p1, 0x428) = S16(me, 0x5a6);
    S32(p1, 0x42c) = S16(me, 0x5a2);
    S32(p1, 0x430) = 0;
}
