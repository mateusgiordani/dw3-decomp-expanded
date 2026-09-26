/*
 * CARDGAME:0x800840f0 CARDGAME_F0x800840f0
 * 152 bytes at CARDGAME.PRO offset 0x1440 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800840f0
 *  Symbols     (none)
 *  Compare     152 bytes from 0x800840f0 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800840f0
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Marks, in the flags at p1+0x46f, each of the 12 hand slots (6 per side,
 * counts p1[0x72c] and p1[0x79e], 14-byte entries) whose entry byte +0xc equals
 * the byte at p1 + (p1[0x575] - 1) * 8 + 0x586; other slots are cleared.
 *
 * Matching note: each side's entry offset is its own single-set variable
 * (o0/o1), so loop.c reduces two constant-folded index givs added to p1 as in
 * PAL; the key row offset `d` is computed before the loop.
 */

#include <stdint.h>

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S8(p, o)     (*(int8_t *)((p) + (o)))

void CARDGAME_F0x800840f0(int32_t p1)
{
    int32_t i;
    int32_t d;
    int32_t e;
    int32_t o0;
    int32_t o1;

    d = (S8(p1, 0x575) - 1) * 8;
    for (i = 0; i < 12; i++) {
        U8(p1 + i, 0x46f) = 0;
        if (i < 6) {
            if (i >= U8(p1, 0x72c))
                continue;
            o0 = i * 0xe + 0x72e;
            e = p1 + o0;
        } else {
            if (i - 6 >= U8(p1, 0x79e))
                continue;
            o1 = (i - 6) * 0xe + 0x7a0;
            e = p1 + o1;
        }
        if (U8(e, 0xc) == U8(p1 + d, 0x586))
            U8(p1 + i, 0x46f) = 1;
    }
}
