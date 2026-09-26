/*
 * CARDGAME:0x800a4924 CARDGAME_F0x800a4924
 * 84 bytes at CARDGAME.PRO offset 0x21c74 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a4924
 *  Symbols     (none)
 *  Compare     84 bytes from 0x800a4924 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a4924
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Returns the index of the first hand card (int16 ids at p1+0x6c8, count
 * p1+0x66e) whose record byte at p1 + (id - 40) * 4 + 0x35c equals `kind`, or
 * the count if none does.
 *
 * Matching note: the record address is a named local, which keeps the -40 bias
 * separate from the scaled index and adds p1 first as in PAL.
 */

#include <stdint.h>

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))

int32_t CARDGAME_F0x800a4924(int32_t p1, int32_t kind)
{
    int32_t i;
    int32_t r;

    for (i = 0; i < S16(p1, 0x66e); i++) {
        r = p1 + (S16(p1 + i * 2, 0x6c8) - 40) * 4;
        if (U8(r, 0x35c) == kind)
            break;
    }
    return i;
}
