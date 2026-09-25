/*
 * STCRDSHP:0x80085cd8 STCRDSHP_func_80085cd8
 * 200 bytes at STCRDSHP.PRO offset 0x3028 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80085cd8
 *  Symbols     D_8004DF98=0x8004df98
 *  Compare     200 bytes from 0x80085cd8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x80085cd8
 */

#include "common/types.h"

extern int32_t (*D_8004DF98)(void);

void STCRDSHP_func_80085cd8(uint8_t *self) {
    int32_t mode;
    int32_t now;

    mode = *(int32_t *)(self + 0x10);
    if (mode == 1) {
        goto mode1;
    }
    if (1 < mode) {
        if (mode == 2) {
            goto mode2;
        }
    }
    return;
mode1:
    if (*(int32_t *)(self + 0x64) != 0) {
        *(int32_t *)(self + 0x64) -= 1;
        ((void (*)(uint8_t *))(*(void (**)(void))(self + 0x3c)))(self);
        *(int32_t *)(self + 0x18) = D_8004DF98();
    } else {
        *(int32_t *)(self + 0x0c) = 3;
    }
    return;
mode2:
    now = D_8004DF98();
    if (1 < now - *(int32_t *)(self + 0x18)) {
        *(int32_t *)(self + 0x10) = 1;
    }
}
