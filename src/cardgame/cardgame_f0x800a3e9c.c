/*
 * CARDGAME:0x800a3e9c CARDGAME_F0x800a3e9c
 * 172 bytes at CARDGAME.PRO offset 0x211ec (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a3e9c
 *  Symbols     (none)
 *  Compare     172 bytes from 0x800a3e9c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a3e9c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Finds the first flagged card of `side` (flags p1+0x446, or p1+0x44c for side
 * 1; count p1 + side * 0x72 + 0x72c) and copies its byte at +0x73a of the
 * 14-byte entry into the slot byte at p1 + p1[0x575] * 8 + 0x586. Returns 1 if
 * a card was flagged.
 *
 * Matching note: the side offset `o` (set in the loop condition) and the slot
 * offset `d` are named so both add base-first as in PAL.
 */

#include <stdint.h>

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S8(p, o)     (*(int8_t *)((p) + (o)))

int32_t CARDGAME_F0x800a3e9c(int32_t p1, int32_t side)
{
    int8_t *flags;
    int32_t ret;
    int32_t i;
    int32_t o;
    int32_t d;

    ret = 0;
    if (side == 1)
        flags = (int8_t *)(p1 + 0x44c);
    else
        flags = (int8_t *)(p1 + 0x446);
    for (i = 0; o = side * 0x72, i < U8(p1 + o, 0x72c); i++) {
        if (flags[i] != 0) {
            ret = 1;
            d = S8(p1, 0x575) * 8;
            U8(p1 + d, 0x586) = U8(p1 + (side * 0x72 + i * 0xe), 0x73a);
            break;
        }
    }
    return ret;
}
