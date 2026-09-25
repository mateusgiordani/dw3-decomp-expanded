/*
 * STCRDABM:0x8008314c STCRDABM_func_8008314c
 * 40 bytes at STCRDABM.PRO offset 0x49c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008314c
 *  Symbols     (none)
 *  Compare     40 bytes from 0x8008314c against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDABM:0x8008314c
 */

#include "common/types.h"

/* STCRDABM:0x8008314c (40 bytes, 10 instructions)
 * PAL-SLES-03936; reviewed body-8008314c, base 0x80082cb0.
 * The method pointer at object offset 0x2c is called with the object and 1.
 */
void STCRDABM_func_8008314c(void *object) {
    ((void (*)(void *, int))*(void **)((char *)object + 0x2c))(object, 1);
}
