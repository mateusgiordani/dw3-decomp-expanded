/*
 * STCRDABM:0x80083a78 STCRDABM_func_80083a78
 * 96 bytes at STCRDABM.PRO offset 0xdc8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80083a78
 *  Symbols     F0x80014504=0x80014504 STCRDABM_func_80083114=0x80083114
 *              STCRDABM_func_8008314c=0x8008314c
 *              STCRDABM_func_80083848=0x80083848
 *  Compare     96 bytes from 0x80083a78 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDABM:0x80083a78
 */

#include "common/types.h"

extern int32_t F0x80014504(int32_t callback, int32_t size, int32_t mode);
extern void STCRDABM_func_80083848(void);
extern void STCRDABM_func_80083114(void);
extern void STCRDABM_func_8008314c(void);

/* STCRDABM:0x80083a78 (96 bytes, 24 instructions); PAL reviewed body. */
void STCRDABM_func_80083a78(int32_t value) {
    int32_t object;

    object = F0x80014504((int32_t)STCRDABM_func_80083848, 0x8c, 0);
    *(int32_t *)(object + 0x84) = (int32_t)STCRDABM_func_80083114;
    *(int32_t *)(object + 0x88) = (int32_t)STCRDABM_func_8008314c;
    *(int32_t *)(object + 0x54) = 0x1000;
    *(int32_t *)(object + 0x58) = 6;
    *(int32_t *)(object + 0x50) = value;
}
