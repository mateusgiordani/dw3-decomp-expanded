/*
 * CARDGAME:0x8008def8 CARDGAME_F0x8008def8
 * 404 bytes at CARDGAME.PRO offset 0xb248 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008def8
 *  Symbols     CARDGAME_F0x8008def8=0x8008def8
 *  Compare     404 bytes from 0x8008def8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008def8
 */

#include "common/types.h"

typedef struct {
    int16_t f0;
    int16_t f2;
    int16_t f4;
    uint8_t pad_06[8];
} CardGameSlot8008def8; /* 0xe bytes */

typedef struct {
    uint8_t pad_000[0x424];
    int32_t f424;
    int32_t f428;
    int32_t f42c;
    int32_t f430;
    int32_t f434;
    uint8_t pad_438[0x46f - 0x438];
    int8_t flags[12]; /* 0x46f */
    uint8_t pad_47b[0x72c - 0x47b];
    uint8_t limit0; /* 0x72c */
    uint8_t pad_72d[3];
    CardGameSlot8008def8 arr0[6]; /* 0x730 */
    uint8_t pad_784[0x79e - 0x784];
    uint8_t limit1; /* 0x79e */
    uint8_t pad_79f[3];
    CardGameSlot8008def8 arr1[6]; /* 0x7a2 */
} CardGameCtx8008def8;

typedef void (*CardGameCb8008def8)(void *self, int32_t index);

typedef struct {
    uint8_t pad_000[0xf30];
    CardGameCb8008def8 cb0; /* +0xf30 */
    CardGameCb8008def8 cb1; /* +0xf34 */
} CardGameArg8008def8;

/*
 * at base 0x80082cb0 (file off 0x5b48).
 *
 * The abs stores negate into temporaries; shi is reused for the limit1 load (a
 * second set, so sched1 gives the shi = t copy no birthing boost) and t is
 * reused as the loop negation temporary (it outlives shi, so cse keeps t
 * canonical and the copy survives).
 */
void CARDGAME_F0x8008def8(CardGameCtx8008def8 *ctx, CardGameArg8008def8 *arg, int32_t packed, int32_t mode)
{
    int32_t t;
    int16_t lo16;
    int32_t shi;
    int32_t slo;
    int32_t hi;
    int32_t pk;
    int32_t two;
    CardGameCb8008def8 cb;
    int32_t i;

    t = packed >> 16;
    shi = t;
    ctx->f424 = 0;
    ctx->f428 = t;
    t = packed << 16;
    lo16 = (int16_t)(t >> 16);
    slo = lo16;
    ctx->f42c = slo;
    ctx->f430 = shi;
    hi = (int16_t)shi;
    pk = packed;
    if (shi < 0) {
        ctx->f430 = -shi;
    }
    ctx->f434 = slo;
    if (slo < 0) {
        ctx->f434 = -slo;
    }
    i = 0;
    two = 2;
    do {
        if (ctx->flags[i] == 0)
            goto next;
        if (i < 6) {
            if (i >= (int32_t)ctx->limit0)
                goto next;
            ctx->arr0[i].f0 += (int16_t)hi;
            ctx->arr0[i].f2 += pk;
            if (mode != two)
                goto call;
            t = ctx->arr0[i].f4;
            ctx->arr0[i].f0 = -t;
            goto call;
        }
        shi = ctx->limit1;
        if (i - 6 >= shi)
            goto next;
        ctx->arr1[i - 6].f0 += (int16_t)hi;
        ctx->arr1[i - 6].f2 += pk;
        if (mode != two)
            goto call;
        t = ctx->arr1[i - 6].f4;
        ctx->arr1[i - 6].f0 = -t;
call:
        if (mode == 0)
            arg->cb0(arg, i);
        else
            arg->cb1(arg, i);
next:;
        ++i;
    } while (i < 12);
}
