/*
 * CARDGAME:0x8009d9e4 CARDGAME_F0x8009d9e4
 * 132 bytes at CARDGAME.PRO offset 0x1ad34 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009d9e4
 *  Symbols     (none)
 *  Compare     132 bytes from 0x8009d9e4 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009d9e4
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Recomputes the deck bounds from the slot states (pairs at p1+0x30a):
 * p1[0x41b] is the first slot from the draw position p1[0x668] whose state
 * exceeds 2 * p1[0x300] + 2 (or 40), and p1[0x41c] is 40 minus the run of
 * trailing slots in state 7.
 *
 * Matching note: the tail scan steps through k = i - 1 in a for loop with a
 * named offset, which keeps the loop unpeeled and the add base-first as in PAL.
 */

#include <stdint.h>

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))

void CARDGAME_F0x8009d9e4(int32_t p1)
{
    int32_t i;
    int32_t k;
    int32_t o;

    for (i = S16(p1, 0x668); i < 40; i++) {
        if (U8(p1 + i * 2, 0x30b) > U8(p1, 0x300) * 2 + 2)
            break;
    }
    U8(p1, 0x41b) = i;
    for (i = 40; i > 0; i = k) {
        k = i - 1;
        o = k * 2;
        if (U8(p1 + o, 0x30b) != 7)
            break;
    }
    U8(p1, 0x41c) = i;
}
