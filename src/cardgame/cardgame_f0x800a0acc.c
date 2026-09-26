/*
 * CARDGAME:0x800a0acc CARDGAME_F0x800a0acc
 * 180 bytes at CARDGAME.PRO offset 0x1de1c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a0acc
 *  Symbols     (none)
 *  Compare     180 bytes from 0x800a0acc against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a0acc
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Totals one side's cards: clears the side record's (p1 + r) sums at
 * +0x59c/+0x59e, then adds each card's values at +0x734/+0x736 of its 14-byte
 * entry (count p1 + side * 0x72 + 0x72c).
 *
 * Matching note: the record offset `r` and the side offset `o` (set in the loop
 * condition) are named so both add base-first as in PAL.
 */

#include <stdint.h>

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))

void CARDGAME_F0x800a0acc(int32_t p1, int32_t side)
{
    int32_t i;
    int32_t o;
    int32_t r;

    r = side * 0xc8;
    S16(p1 + r, 0x59c) = 0;
    S16(p1 + r, 0x59e) = 0;
    for (i = 0; o = side * 0x72, i < U8(p1 + o, 0x72c); i++) {
        S16(p1 + r, 0x59c) += S16(p1 + (side * 0x72 + i * 0xe), 0x734);
        S16(p1 + r, 0x59e) += S16(p1 + (side * 0x72 + i * 0xe), 0x736);
    }
}
