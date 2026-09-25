/*
 * STCRDABM:0x80082f54 STCRDABM_func_80082f54
 * 180 bytes at STCRDABM.PRO offset 0x2a4 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80082f54
 *  Symbols     STCRDABM_func_80082e10=0x80082e10
 *  Compare     180 bytes from 0x80082f54 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDABM:0x80082f54
 */

#include "common/types.h"

extern void STCRDABM_func_80082e10(int32_t object);

/* STCRDABM:0x80082f54 (180 bytes, 45 instructions); PAL reviewed body. */
void STCRDABM_func_80082f54(int32_t object) {
    int32_t status;
    int32_t value;

    status = *(int32_t *)(object + 0xc);
    if (status != 1) {
        if (status > 1) {
            if (status == 2) {
                goto update;
            }
            if (status == 3) {
                return;
            }
        }
        ((void (*)(int32_t))*(void **)(object + 0x38))(object);
        return;
    }
    if (*(int32_t *)(object + 0x10) == 0) {
        return;
    }
    value = *(int32_t *)(object + 0x5c) + *(int32_t *)(object + 0x60);
    *(int32_t *)(object + 0x5c) = value;
    if (*(int32_t *)(object + 0x58) == 0) {
        if (value < 0xff01) {
            goto update;
        }
        *(int32_t *)(object + 0x5c) = 0xff00;
        *(int32_t *)(object + 0xc) = 2;
        goto update;
    }
    if (value >= 0) {
        goto update;
    }
    *(int32_t *)(object + 0x5c) = 0;
    *(int32_t *)(object + 0xc) = 2;
update:
    STCRDABM_func_80082e10(object);
}
