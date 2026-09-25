/*
 * STCRDABM:0x80083734 STCRDABM_func_80083734
 * 76 bytes at STCRDABM.PRO offset 0xa84 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80083734
 *  Symbols     D0x80048d34=0x80048d34
 *  Compare     76 bytes from 0x80083734 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDABM:0x80083734
 */

#include "common/types.h"

extern uint8_t D0x80048d34[];

/* STCRDABM:0x80083734 (76 bytes, 19 instructions)
 * PAL-SLES-03936; reviewed body-80083734, base 0x80082cb0.
 */
int32_t STCRDABM_func_80083734(int32_t object) {
    int32_t index;

    index = 0;
    object = *(int32_t *)(object + 0x5c);
    do {
        if (D0x80048d34[object + index + 0x4df] != 0 ||
            object + index >= 0x13b) {
            return 1;
        }
        index++;
    } while (index < 0xc);
    return 0;
}
