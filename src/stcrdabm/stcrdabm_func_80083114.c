/*
 * STCRDABM:0x80083114 STCRDABM_func_80083114
 * 56 bytes at STCRDABM.PRO offset 0x464 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80083114
 *  Symbols     (none)
 *  Compare     56 bytes from 0x80083114 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDABM:0x80083114
 */

#include "common/types.h"

/* STCRDABM:0x80083114 (56 bytes, 14 instructions)
 * PAL-SLES-03936; reviewed body-80083114, base 0x80082cb0.
 */
void STCRDABM_func_80083114(void *object, int32_t value) {
    int32_t previous;

    previous = *(int32_t *)((char *)object + 0x5c);
    *(int32_t *)((char *)object + 0x5c) = value;
    *(int32_t *)((char *)object + 0x60) = previous;
    *(int32_t *)((char *)object + 0x68) = 0;
    *(int32_t *)((char *)object + 0x70) = 0;
    ((void (*)(void *, int32_t))*(void **)((char *)object + 0x28))(object, 2);
}
