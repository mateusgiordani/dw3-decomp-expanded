/*
 * CARDGAME:0x8009df68 CARDGAME_F0x8009df68
 * 188 bytes at CARDGAME.PRO offset 0x1b2b8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009df68
 *  Symbols     (none)
 *  Compare     188 bytes from 0x8009df68 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009df68
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Sets up the counter block at p1+0x498: its target (+0x3c) is the side's
 * 0xc8-byte record value selected by `kind` (0: +0x5a6, 1: +0x5a2, 2: +0x5a4),
 * clears +0x30..+0x38 and sets +0x40 to target * 4 + 10.
 *
 * Matching note: the record offset `o` is named in each case so p1 + o adds
 * base-first as in PAL.
 */

#include <stdint.h>

#define S16(p, o)    (*(int16_t *)((p) + (o)))
#define S32(p, o)    (*(int32_t *)((p) + (o)))

void CARDGAME_F0x8009df68(int32_t p1, int32_t p2, int32_t side, int32_t kind)
{
    int32_t c;
    int32_t o;

    c = p1 + 0x498;
    switch (kind) {
    case 0:
        o = side * 0xc8;
        S32(c, 0x3c) = S16(p1 + o, 0x5a6);
        break;
    case 1:
        o = side * 0xc8;
        S32(c, 0x3c) = S16(p1 + o, 0x5a2);
        break;
    case 2:
        o = side * 0xc8;
        S32(c, 0x3c) = S16(p1 + o, 0x5a4);
        break;
    }
    S32(c, 0x30) = 0;
    S32(c, 0x34) = 0;
    S32(c, 0x38) = 0;
    S32(c, 0x40) = S32(c, 0x3c) * 4 + 10;
}
