/*
 * STCRDABM:0x800858a0 STCRDABM_func_800858a0
 * 148 bytes at STCRDABM.PRO offset 0x2bf0 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800858a0
 *  Symbols     D0x80055c48=0x80055c48
 *  Compare     148 bytes from 0x800858a0 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDABM:0x800858a0
 */

#include "common/types.h"

extern int32_t (*D0x80055c48)(int32_t);

/* STCRDABM:0x800858a0 (148 bytes, 37 instructions); PAL reviewed body. */
void STCRDABM_func_800858a0(int32_t *object, int32_t direction) {
    object[3] = 1;
    /* Branch-local results avoid the shared GCC pseudo's v0/v1 conflicts. */
    if (direction != 0) {
        int32_t step;
        D0x80055c48(0x40019);
        step = 0x1000 / object[0];
        object[2] = 0;
        object[1] = step;
    } else {
        int32_t origin;
        int32_t step;
        D0x80055c48(0x4001a);
        origin = 0x1000;
        object[2] = origin;
        step = (origin / object[0]) * -2;
        object[1] = step;
    }
}
