/*
 * STCRDABM:0x80085934 STCRDABM_func_80085934
 * 108 bytes at STCRDABM.PRO offset 0x2c84 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80085934
 *  Symbols     (none)
 *  Compare     108 bytes from 0x80085934 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDABM:0x80085934
 */

#include "common/types.h"

/* STCRDABM:0x80085934 (108 bytes, 27 instructions)
 * PAL-SLES-03936; reviewed body-80085934, base 0x80082cb0.
 */
int32_t STCRDABM_func_80085934(int32_t object) {
    int32_t value;

    if (*(int32_t *)(object + 0xc) == 0) {
        return 1;
    }
    value = *(int32_t *)(object + 8) + *(int32_t *)(object + 4);
    *(int32_t *)(object + 8) = value;
    if (*(int32_t *)(object + 4) > 0) {
        if (value > 0x1000) {
            *(int32_t *)(object + 8) = 0x1000;
            *(int32_t *)(object + 0xc) = 0;
            return 1;
        }
    } else if (value < 0) {
        *(int32_t *)(object + 8) = 0;
        *(int32_t *)(object + 0xc) = 0;
        return 1;
    }
    return 0;
}
