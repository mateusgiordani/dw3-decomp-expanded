/*
 * CARDGAME:0x8009f8c0 CARDGAME_F0x8009f8c0
 * 80 bytes at CARDGAME.PRO offset 0x1cc10 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009f8c0
 *  Symbols     (none)
 *  Compare     80 bytes from 0x8009f8c0 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009f8c0
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * On the first call (p1[0x2f9] == 0) sets p1[0x421] = 0xa7, state p1[0x2f4] = 1
 * and p1[0x2f9] = 1, returning 0; afterwards sets p1[0x2f5] to whether the
 * target slot p1[0x440] is nonzero and returns 1.
 */

#include <stdint.h>

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S32(p, o)    (*(int32_t *)((p) + (o)))

int32_t CARDGAME_F0x8009f8c0(int32_t p1)
{
    int32_t ret;

    ret = 0;
    if (U8(p1, 0x2f9) == 0) {
        U8(p1, 0x421) = 0xa7;
        U8(p1, 0x2f4) = 1;
        U8(p1, 0x2f9) = 1;
    } else {
        if (S32(p1, 0x440) == 0)
            U8(p1, 0x2f5) = 0;
        else
            U8(p1, 0x2f5) = 1;
        ret = 1;
    }
    return ret;
}
