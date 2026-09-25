/*
 * STCRDABM:0x80083780 STCRDABM_func_80083780
 * 200 bytes at STCRDABM.PRO offset 0xad0 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80083780
 *  Symbols     D0x8004df98=0x8004df98
 *  Compare     200 bytes from 0x80083780 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDABM:0x80083780
 */

#include "common/types.h"

extern int32_t (*D0x8004df98)(void);

/* STCRDABM:0x80083780 (200 bytes), PAL-SLES-03936. */
void STCRDABM_func_80083780(void *object) {
    int32_t state;
    int32_t value;

    state = *(int32_t *)((char *)object + 0x10);
    if (state == 1)
        goto do_state1;
    if (state < 2)
        goto done;
    if (state == 2)
        goto do_state2;
    goto done;
do_state1:
    value = *(int32_t *)((char *)object + 0x64);
    if (value == 0)
        goto zero;
    *(int32_t *)((char *)object + 0x64) = value - 1;
    ((void (*)(void *))*(void **)((char *)object + 0x3c))(object);
    *(int32_t *)((char *)object + 0x18) = D0x8004df98();
    goto done;
zero:
    *(int32_t *)((char *)object + 0x0c) = 3;
    goto done;
do_state2:
    value = D0x8004df98();
    if (value - *(int32_t *)((char *)object + 0x18) > 1)
        *(int32_t *)((char *)object + 0x10) = 1;
done:
    ;
}
