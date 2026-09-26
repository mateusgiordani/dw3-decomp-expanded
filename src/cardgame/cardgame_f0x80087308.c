/*
 * CARDGAME:0x80087308 CARDGAME_F0x80087308
 * 84 bytes at CARDGAME.PRO offset 0x4658 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80087308
 *  Symbols     (none)
 *  Compare     84 bytes from 0x80087308 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80087308
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Returns 0 when fewer than 6 cards are selected (p1[0x444]) and any of the
 * p3[0xa] hand cards is flagged (p1+0x446); returns 1 otherwise.
 */

#include <stdint.h>

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S8(p, o)     (*(int8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))

int32_t CARDGAME_F0x80087308(int32_t p1, int32_t p2, int32_t p3)
{
    int32_t ret;
    int32_t i;

    ret = 1;
    if (U8(p1, 0x444) < 6) {
        for (i = 0; i < S16(p3, 0xa); i++) {
            if (S8(p1 + i, 0x446) != 0) {
                ret = 0;
                break;
            }
        }
    }
    return ret;
}
