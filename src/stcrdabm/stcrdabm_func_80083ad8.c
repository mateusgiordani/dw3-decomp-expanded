/*
 * STCRDABM:0x80083ad8 STCRDABM_func_80083ad8
 * 248 bytes at STCRDABM.PRO offset 0xe28 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80083ad8
 *  Symbols     D0x8004de10=0x8004de10 STCRDABM_func_8008566c=0x8008566c
 *  Compare     248 bytes from 0x80083ad8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDABM:0x80083ad8
 */

#include "common/types.h"

extern int32_t D0x8004de10[];
extern int32_t STCRDABM_func_8008566c(void);

/* STCRDABM:0x80083ad8 (248 bytes, 62 instructions); PAL reviewed body. */
void STCRDABM_func_80083ad8(int32_t task, int32_t *out) {
    int32_t state;
    int32_t *sys;
    int16_t params[4];
    int32_t handle;

    state = *(int32_t *)(task + 0xc);
    if (state == 0) goto work;
    if (state < 0) goto work;
    if (state < 4) return;
work:
    sys = D0x8004de10;
    ((void (*)(void))sys[0x54])();
    ((void (*)(int32_t))sys[0x55])(0xf000);
    ((void (*)(int32_t, int32_t, int32_t, int32_t))sys[0x5d])(0x140, 0xf0, 0, 0);
    params[0] = 0;
    params[1] = 0;
    params[2] = 0x140;
    params[3] = 0xf0;
    handle = ((int32_t (*)(int16_t *, int32_t, int32_t))sys[0x5b])(params, 3, 0x1000);
    ((void (*)(int32_t, int32_t, int32_t, int32_t))*(int32_t *)(handle + 0x12c))(handle, 0, 0, 0);
    *out = STCRDABM_func_8008566c();
    ((void (*)(int32_t))*(int32_t *)(task + 0x38))(task);
}
