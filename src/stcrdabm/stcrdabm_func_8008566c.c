/*
 * STCRDABM:0x8008566c STCRDABM_func_8008566c
 * 60 bytes at STCRDABM.PRO offset 0x29bc (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008566c
 *  Symbols     F0x80014504=0x80014504 STCRDABM_func_80085540=0x80085540
 *  Compare     60 bytes from 0x8008566c against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDABM:0x8008566c
 */

#include "common/types.h"

/* STCRDABM:0x8008566c (60 bytes, 15 instructions)
 * PAL-SLES-03936; reviewed body-8008566c, base 0x80082cb0.
 */
extern int32_t F0x80014504(int32_t callback, int32_t size, int32_t mode);
extern void STCRDABM_func_80085540(void);

void STCRDABM_func_8008566c(void) {
    int32_t object;

    object = F0x80014504((int32_t)STCRDABM_func_80085540, 0xd8, 0x4c);
    *(int32_t *)(object + 0x50) = 0x1000;
    *(int32_t *)(object + 0x54) = 7;
}
