/*
 * CARDGAME:0x8009d8fc CARDGAME_F0x8009d8fc
 * 232 bytes at CARDGAME.PRO offset 0x1ac4c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009d8fc
 *  Symbols     (none)
 *  Compare     232 bytes from 0x8009d8fc against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009d8fc
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Sorts the hand list (int16 card ids at p1+0x6c8, count p1+0x66e) in place,
 * ascending by each card's byte at p1 + (id - 40) * 4 + 0x35c and then by its
 * int16 at +0x35e (exchange sort).
 *
 * Matching note: `swap` is cleared in the inner loop's init and step (not at
 * the top of the body), which keeps it out of the compare block as in PAL.
 */

#include <stdint.h>

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))

void CARDGAME_F0x8009d8fc(int32_t p1)
{
    int32_t i;
    int32_t j;
    int32_t swap;
    int32_t a;
    int32_t b;
    int32_t ka;
    int32_t kb;
    int32_t va;
    int32_t vb;
    int32_t t;

    for (i = 0; i < S16(p1, 0x66e) - 1; i++) {
        for (j = i + 1, swap = 0; j < S16(p1, 0x66e); j++, swap = 0) {
            a = p1 + (S16(p1 + i * 2, 0x6c8) - 40) * 4;
            b = p1 + (S16(p1 + j * 2, 0x6c8) - 40) * 4;
            ka = U8(a, 0x35c);
            kb = U8(b, 0x35c);
            va = S16(a, 0x35e);
            vb = S16(b, 0x35e);
            if (kb < ka || (ka == kb && vb < va))
                swap = 1;
            if (swap) {
                t = S16(p1 + i * 2, 0x6c8);
                S16(p1 + i * 2, 0x6c8) = S16(p1 + j * 2, 0x6c8);
                S16(p1 + j * 2, 0x6c8) = t;
            }
        }
    }
}
