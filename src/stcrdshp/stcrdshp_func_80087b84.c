/*
 * STCRDSHP:0x80087b84 STCRDSHP_func_80087b84
 * 88 bytes at STCRDSHP.PRO offset 0x4ed4 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80087b84
 *  Symbols     EXE_F0x80014504=0x80014504 STCRDSHP_func_80087a2c=0x80087a2c
 *  Compare     88 bytes from 0x80087b84 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x80087b84
 */

#include "common/types.h"

extern void STCRDSHP_func_80087a2c(void);
extern void *EXE_F0x80014504(void *, int32_t, int32_t);

void STCRDSHP_func_80087b84(void *callback, void *user_data) {
    uint8_t *object;

    object = EXE_F0x80014504(&STCRDSHP_func_80087a2c, 0xdc, 0x4c);
    *(int32_t *)(object + 0x54) = 0x1000;
    *(int32_t *)(object + 0x58) = 6;
    *(void **)(object + 0x50) = callback;
    *(void **)(object + 0xa0) = user_data;
}
