/*
 * STCRDSHP:0x80088afc STCRDSHP_func_80088afc
 * 84 bytes at STCRDSHP.PRO offset 0x5e4c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80088afc
 *  Symbols     D_80048DA0=0x80048da0
 *  Compare     84 bytes from 0x80088afc against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x80088afc
 */

#include "common/types.h"

extern uint32_t D_80048DA0;

void STCRDSHP_func_80088afc(void *self) {
    uint8_t *sub = *(uint8_t **)((uint8_t *)self + 0x24);
    uint8_t *object = *(uint8_t **)(sub + 0x10);

    ((void (*)(void *, int32_t, uint32_t))(*(void (**)(void))(object + 0x118)))
        (object, 0, D_80048DA0);
    object = *(uint8_t **)(sub + 0x10);
    ((void (*)(void *, int32_t))(*(void (**)(void))(object + 0x148)))(object, 1);
}
