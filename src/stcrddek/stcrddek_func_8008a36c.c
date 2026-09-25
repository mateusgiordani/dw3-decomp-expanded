/*
 * STCRDDEK:0x8008a36c STCRDDEK_func_8008a36c
 * 108 bytes at STCRDDEK.PRO offset 0x76bc (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008a36c
 *  Symbols     (none)
 *  Compare     108 bytes from 0x8008a36c against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x8008a36c
 */

#include <stdint.h>

int32_t STCRDDEK_func_8008a36c(int32_t *p)
{
    int32_t sum;

    if (p[3] == 0)
        return 1;
    sum = p[2] + p[1];
    p[2] = sum;
    if (p[1] > 0) {
        if (sum > 0x1000) {
            p[2] = 0x1000;
            p[3] = 0;
            return 1;
        }
    } else if (sum < 0) {
        p[2] = 0;
        p[3] = 0;
        return 1;
    }
    return 0;
}
