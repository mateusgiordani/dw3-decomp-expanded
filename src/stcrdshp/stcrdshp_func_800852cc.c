/*
 * STCRDSHP:0x800852cc STCRDSHP_func_800852cc
 * 160 bytes at STCRDSHP.PRO offset 0x261c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800852cc
 *  Symbols     STCRDSHP_func_8008300c=0x8008300c
 *              STCRDSHP_func_80083ba8=0x80083ba8
 *              STCRDSHP_func_800843c0=0x800843c0
 *              STCRDSHP_func_800844b8=0x800844b8
 *  Compare     160 bytes from 0x800852cc against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x800852cc
 */

#include "common/types.h"

typedef void (*STCRDSHP_Callback)(void *object);

extern void STCRDSHP_func_800844b8(void *object, int32_t arg);
extern void STCRDSHP_func_80083ba8(void *object);
extern void STCRDSHP_func_8008300c(void *object, int32_t arg);
extern void STCRDSHP_func_800843c0(void *);

void STCRDSHP_func_800852cc(void *object, int32_t arg) {
    int32_t state = *(int32_t *)((char *)object + 0x0c);

    if (state == 1)
        goto state1;
    if (state < 2)
        goto initialize;
    if (state > 3)
        goto initialize;
    return;
initialize:
    ((STCRDSHP_Callback)*(int32_t *)((char *)object + 0x38))(object);
    STCRDSHP_func_8008300c(object, arg);
    *(int32_t *)((char *)object + 0x720) = 10;
    *(int32_t *)((char *)object + 0x710) = 10;
    *(int32_t *)((char *)object + 0x6f0) = 10;
    *(int32_t *)((char *)object + 0x700) = 10;
    STCRDSHP_func_800843c0(object);
    return;
state1:
    STCRDSHP_func_800844b8(object, arg);
    STCRDSHP_func_80083ba8(object);
}
