/*
 * CARDGAME:0x8009f910 CARDGAME_F0x8009f910
 * 136 bytes at CARDGAME.PRO offset 0x1cc60 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009f910
 *  Symbols     (none)
 *  Compare     136 bytes from 0x8009f910 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009f910
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * On the first call (p1[0x2f9] == 0) sets p1[0x421] = 20 and state p1[0x2f4] =
 * 1 and bumps p1[0x2f9], returning 0; afterwards maps the target slot p1[0x440]
 * 0/1/2 to 1/2/3 (0 otherwise).
 */

#include <stdint.h>

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S32(p, o)    (*(int32_t *)((p) + (o)))

int32_t CARDGAME_F0x8009f910(int32_t p1)
{
    int32_t ret;

    ret = 0;
    if (U8(p1, 0x2f9) == 0) {
        U8(p1, 0x421) = 20;
        U8(p1, 0x2f4) = 1;
        U8(p1, 0x2f9)++;
    } else {
        switch (S32(p1, 0x440)) {
        case 0:
            ret = 1;
            break;
        case 1:
            ret = 2;
            break;
        case 2:
            ret = 3;
            break;
        }
    }
    return ret;
}
