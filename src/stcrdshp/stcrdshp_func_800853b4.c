/*
 * STCRDSHP:0x800853b4 STCRDSHP_func_800853b4
 * 136 bytes at STCRDSHP.PRO offset 0x2704 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800853b4
 *  Symbols     (none)
 *  Compare     136 bytes from 0x800853b4 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x800853b4
 */

#include "common/types.h"

void STCRDSHP_func_800853b4(void *self, int32_t direction, int32_t duration) {
    uint8_t *state = self;
    ((void (*)(void *, int32_t))(*(void (**)(void *))(state + 0x28)))(self, 1);
    *(int32_t *)(state + 0x10) = 1;
    *(int32_t *)(state + 0x58) = direction;
    if (direction == 0) {
        int32_t step;
        step = 0xff00 / duration;
        *(int32_t *)(state + 0x5c) = 0;
        *(int32_t *)(state + 0x60) = step;
    } else {
        int32_t step;
        step = -(0xff00 / duration);
        *(int32_t *)(state + 0x5c) = 0xff00;
        *(int32_t *)(state + 0x60) = step;
    }
}
