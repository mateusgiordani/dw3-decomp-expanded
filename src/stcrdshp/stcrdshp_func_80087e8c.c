/*
 * STCRDSHP:0x80087e8c STCRDSHP_func_80087e8c
 * 288 bytes at STCRDSHP.PRO offset 0x51dc (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80087e8c
 *  Symbols     D_80044B38=0x80044b38 D_80048DA0=0x80048da0
 *              D_8005CCA8=0x8005cca8
 *  Compare     288 bytes from 0x80087e8c against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x80087e8c
 */

#include "common/types.h"

/*
 * Next function STCRDSHP:0x80087fac starts immediately after (no gap/overlap).
 *
 * All 72 words match the PAL bytes.
 *
 * Callers: STCRDSHP:0x8008842c (flag=1), STCRDSHP:0x80088a30 (flag=0), both
 * pass (a0=s3 struct base, a1=s4 object-pointer array, a2=flag).
 *
 * File name does not prove a card-shop-menu role; semantics unconfirmed, names
 * conservative.
 */
extern struct {
    char pad[0x414];
    int32_t (*fn414)(int32_t);
} D_80044B38;
extern int32_t D_8005CCA8;
extern uint32_t D_80048DA0;

void STCRDSHP_func_80087e8c(void *p1, void **objs, int32_t flag) {
    int32_t r;
    void *obj;

    if (flag != 0) {
        r = D_80044B38.fn414(D_8005CCA8 + 0x94);
        obj = objs[0];
        ((void (*)(void *, int32_t, int32_t))(*(void (**)(void))((uint8_t *)obj + 0x114)))(obj, r, *(int32_t *)((uint8_t *)p1 + 100));
        r = D_80044B38.fn414(D_8005CCA8 + 0x32);
        obj = objs[3];
        ((void (*)(void *, int32_t, int32_t))(*(void (**)(void))((uint8_t *)obj + 0x114)))(obj, r, 3);
        obj = objs[4];
        ((void (*)(void *, int32_t, uint32_t))(*(void (**)(void))((uint8_t *)obj + 0x118)))(obj, 0, D_80048DA0);
        obj = objs[4];
        ((void (*)(void *, int32_t))(*(void (**)(void))((uint8_t *)obj + 0x148)))(obj, 1);
    } else {
        obj = objs[0];
        ((void (*)(void *, int32_t))(*(void (**)(void))((uint8_t *)obj + 0x144)))(obj, 0);
        obj = objs[3];
        ((void (*)(void *, int32_t))(*(void (**)(void))((uint8_t *)obj + 0x144)))(obj, 0);
        obj = objs[4];
        ((void (*)(void *, int32_t))(*(void (**)(void))((uint8_t *)obj + 0x144)))(obj, 0);
    }
}
