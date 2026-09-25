/*
 * STCRDSHP:0x80088ca0 STCRDSHP_func_80088ca0
 * 388 bytes at STCRDSHP.PRO offset 0x5ff0 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80088ca0
 *  Symbols     D_80044B38=0x80044b38 D_80048D34=0x80048d34
 *              D_8004B448=0x8004b448 D_8005CCA8=0x8005cca8
 *              D_8008CB28=0x8008cb28 EXE_F0x80014504=0x80014504
 *              STCRDSHP_func_80088afc=0x80088afc
 *              STCRDSHP_func_80088b50=0x80088b50
 *  Compare     388 bytes from 0x80088ca0 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x80088ca0
 */

#include "common/types.h"

/*
 * The sentinel-table loop indexes D_8008CB28 by counter so the optimizer
 * derives the cursor (late base completion in the entry-branch delay slot); the
 * service base is a local (lui/addiu pair, lui in the 0x400-branch delay slot).
 * Signature of STCRDSHP_func_80088b50 is not yet recovered (address-only use
 * below). Caller STCRDSHP_func_80087bdc stores the return value through its
 * second parameter.
 */
extern void *EXE_F0x80014504(void *, int32_t, int32_t);
extern void STCRDSHP_func_80088afc(void *);
extern void STCRDSHP_func_80088b50(void);

extern uint8_t D_80048D34[];
extern uint8_t D_80044B38[];
extern int32_t (*D_8004B448)(void);
extern int32_t D_8005CCA8;
extern int32_t D_8008CB28[];

void *STCRDSHP_func_80088ca0(void) {
    uint8_t *ctx;
    uint8_t *ex;
    uint8_t *svc;
    int32_t sel;
    int32_t i;

    ctx = (uint8_t *)EXE_F0x80014504((void *)STCRDSHP_func_80088b50, 0x3cc, 0x2c);
    *(void **)(ctx + 0x3c8) = (void *)STCRDSHP_func_80088afc;
    *(int32_t *)(ctx + 0x50) = 0x1000;
    *(int32_t *)(ctx + 0x54) = 7;
    ex = D_80048D34;
    *(int32_t *)(ctx + 0x60) = (*(int32_t (**)(void))(ex + 0x2708))();
    sel = *(int32_t *)(ex + 0x34);
    i = 0;
    while (D_8008CB28[i] != 0) {
        int32_t *row = D_8008CB28 + i;
        if (row[0] == sel)
            *(int32_t *)(ctx + 0x64) = row[1];
        i += 2;
    }
    if (*(int32_t *)(ctx + 0x64) == 0)
        *(int32_t *)(ctx + 0x64) = 0x1f;
    if (D_8004B448() == 0x400)
        *(int32_t *)(ctx + 0x6c) = 2;
    svc = D_80044B38;
    (*(void (**)(int32_t))(svc + 0x40c))(0x7f6);
    (*(void (**)(int32_t))(svc + 0x40c))(0x7f7);
    (*(void (**)(int32_t))(svc + 0x40c))(0x7f8);
    (*(void (**)(int32_t))(svc + 0x40c))(0x7f9);
    (*(void (**)(int32_t))(svc + 0x40c))(0x7fa);
    (*(void (**)(int32_t))(D_80044B38 + 0x40c))(D_8005CCA8 + 0x16);
    (*(void (**)(int32_t))(D_80044B38 + 0x40c))(D_8005CCA8 + 0x1d);
    return ctx;
}
