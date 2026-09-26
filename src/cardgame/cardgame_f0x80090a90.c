/*
 * CARDGAME:0x80090a90 CARDGAME_F0x80090a90
 * 272 bytes at CARDGAME.PRO offset 0xdde0 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80090a90
 *  Symbols     (none)
 *  Compare     272 bytes from 0x80090a90 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80090a90
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Marks p1[0x444] card slots in the flag bytes at p1+0x46f. Mode 0 marks the
 * run starting at p1[0x5a0]; if it would pass slot 40 the count is cut to what
 * fit and p1[0x445] is set. Otherwise the count is capped by p1[0x66c] (also
 * setting p1[0x445]) and slots are taken from the draw position p1[0x668]
 * upwards while their state (pairs at p1+0x30a) equals 2 * p1[0x300] + 2, and
 * from slot 39 downwards otherwise. Always returns 1.
 *
 * Matching note: the mode-0 exit jumps to the shared return, so dbr leaves the
 * final jr delay slot empty as in PAL.
 */

#include <stdint.h>

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))

int32_t CARDGAME_F0x80090a90(int32_t p1, int32_t p2, int32_t mode)
{
    int32_t i;
    int32_t j;
    int32_t k;
    int32_t n;

    if (U8(p1, 0x444) != 0) {
        if (mode == 0) {
            i = S16(p1, 0x5a0);
            n = 0;
            if (i < i + U8(p1, 0x444)) {
                do {
                    if (i >= 40)
                        goto over;
                    U8(p1 + i, 0x46f) = 1;
                    n++;
                } while (++i < S16(p1, 0x5a0) + U8(p1, 0x444));
                goto done;
            over:
                U8(p1, 0x445) = 1;
                U8(p1, 0x444) = n;
            }
        } else {
            k = S16(p1, 0x668);
            j = 39;
            if (U8(p1, 0x444) > S16(p1, 0x66c)) {
                U8(p1, 0x445) = 1;
                U8(p1, 0x444) = U8(p1, 0x66c);
            }
            for (i = 0; i < U8(p1, 0x444); i++) {
                if (U8(p1 + k * 2, 0x30b) == U8(p1, 0x300) * 2 + 2) {
                    U8(p1 + k, 0x46f) = 1;
                    k++;
                } else {
                    U8(p1 + j, 0x46f) = 1;
                    j--;
                }
            }
        }
    }
done:
    return 1;
}
