/*
 * STCRDABM:0x80083008 STCRDABM_func_80083008
 * 68 bytes at STCRDABM.PRO offset 0x358 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80083008
 *  Symbols     F0x80014504=0x80014504 STCRDABM_func_80082d88=0x80082d88
 *              STCRDABM_func_80082f54=0x80082f54
 *  Compare     68 bytes from 0x80083008 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDABM:0x80083008
 */

#include "common/types.h"

/* STCRDABM:0x80083008 (68 bytes, 17 instructions)
 * PAL-SLES-03936; reviewed body-80083008, base 0x80082cb0.
 */
extern int32_t F0x80014504(int32_t callback, int32_t size, int32_t mode);
extern void STCRDABM_func_80082f54(void);
extern void STCRDABM_func_80082d88(void);

void STCRDABM_func_80083008(void) {
    int32_t object;

    object = F0x80014504((int32_t)STCRDABM_func_80082f54, 0x68, 0);
    *(int32_t *)(object + 0x64) = (int32_t)STCRDABM_func_80082d88;
    *(int32_t *)(object + 0x50) = 0x1000;
    *(int32_t *)(object + 0x54) = 0;
}
