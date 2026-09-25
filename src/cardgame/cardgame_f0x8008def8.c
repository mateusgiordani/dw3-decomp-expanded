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

/* CARDGAME:0x8008def8 (404 bytes, body 404) at base 0x80082cb0 (file off 0x5b48).
 * Callers (Ghidra read-only, project ddw3-pal-sles-03936 program CARDGAME):
 * x-ref to 0x8008def8 from CARDGAME_F0x80084320 at 0x800848d8/0x80084928/0x80084944.
 * Match: exact_byte_match 404/404 with psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base).
 * The abs stores negate into temporaries; shi is reused for the limit1 load (a second
 * set, so sched1 gives the shi = t copy no birthing boost) and t is reused as the loop
 * negation temporary (it outlives shi, so cse keeps t canonical and the copy survives).
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
