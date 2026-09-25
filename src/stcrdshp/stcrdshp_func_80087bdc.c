/*
 * STCRDSHP:0x80087bdc STCRDSHP_func_80087bdc
 * 248 bytes at STCRDSHP.PRO offset 0x4f2c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80087bdc
 *  Symbols     D_8004DE10=0x8004de10 STCRDSHP_func_80088ca0=0x80088ca0
 *  Compare     248 bytes from 0x80087bdc against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x80087bdc
 */

#include "common/types.h"

extern uint32_t D_8004DE10[];
extern void *STCRDSHP_func_80088ca0(void);

typedef struct {
    uint8_t pad[0xc];
    int32_t state;
    uint8_t pad2[0x28];
    void (*notify)(void *self);
} STCRDSHP_87bdc_ctx;

void STCRDSHP_func_80087bdc(STCRDSHP_87bdc_ctx *ctx, void **out) {
    uint8_t *object;
    int16_t rect[4];

    if (ctx->state == 0)
        goto init;
    if (ctx->state < 0)
        goto init;
    if (ctx->state < 4)
        goto done;
init:
    ((void (*)(void))D_8004DE10[0x54])();
    ((void (*)(int32_t))D_8004DE10[0x55])(0xf000);
    ((void (*)(int32_t, int32_t, int32_t, int32_t))D_8004DE10[0x5d])(0x140, 0xf0, 0, 0);
    rect[0] = 0;
    rect[1] = 0;
    rect[2] = 0x140;
    rect[3] = 0xf0;
    object = ((void *(*)(int16_t *, int32_t, int32_t))D_8004DE10[0x5b])(rect, 3, 0x1000);
    ((void (*)(void *, int32_t, int32_t, int32_t))(*(void **)(object + 0x12c)))(object, 0, 0, 0);
    *out = STCRDSHP_func_80088ca0();
    ctx->notify(ctx);
done: ;
}
