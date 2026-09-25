#include "common/types.h"

/* STCRDDEK:0x80083d28, PAL-SLES-03936, 1360-byte body.
 * GCC 2.8.1 / ASPSX 2.79, O2 with -fno-strength-reduce.
 * Per-path final calls preserve the PAL tails before compiler cross-jumping.
 * See docs/c-matching-guide/submissions/stcrddek-80083d28/strategy-r5.md for measured controls. */

extern void F0x8001ebf8(void *buf);

extern struct {
    char pad[0x40c];
    void (*fn40c)(int32_t a0);
    char pad2[0x4];
    int32_t (*fn414)(int32_t a0);
} D_80044B38;

extern int32_t D_8005CCA8;

/* Absolute record table at 0x8004935c (lui 0x8005 + addiu -0x6ca4 in PAL);
 * bounds and element type unknown, indexed here by rec * 0x66. */
extern uint8_t D_8004935C[];
/* lui 0x8005 + addiu -0x72cc, plus lh +0x63e */
extern uint8_t D_80048D34[];
/* Access view only: the field offset is known; full record layout is not. */
typedef struct {
    uint8_t pad[0x63e];
    int16_t value;
} TableValue83d28;

typedef struct {
    uint8_t pad_00[0x50];
    int32_t f50; /* +0x50 element-list base */
    uint8_t pad_54[0x08];
    int32_t f5c; /* +0x5c record index */
    int32_t f60; /* +0x60 table addend */
    int32_t f64; /* +0x64 table addend */
    uint8_t pad_68[0x3D4];
    int32_t f43c; /* +0x43c mode/extra flag */
    int32_t f440; /* +0x440 threshold compare vs 0x3c */
} Stcrddek83d28Ctx;

#define OBJ_M114(o, a, b) \
    ((void (*)(void *, int32_t, int32_t))(*(void **)((uint8_t *)(o) + 0x114)))((o), (a), (b))
#define OBJ_M118(o, a, b) \
    ((void (*)(void *, int32_t, int32_t))(*(void **)((uint8_t *)(o) + 0x118)))((o), (a), (b))
#define OBJ_M144(o, a) \
    ((void (*)(void *, int32_t))(*(void **)((uint8_t *)(o) + 0x144)))((o), (a))
#define OBJ_M148(o, a) \
    ((void (*)(void *, int32_t))(*(void **)((uint8_t *)(o) + 0x148)))((o), (a))

void STCRDDEK_func_80083d28(Stcrddek83d28Ctx *ctx, void **objs, int32_t flag)
{
    int32_t i;
    int32_t tmp;
    int32_t s0;
    int32_t ret;
    int32_t tableBase;
    int32_t tableOffset;
    int32_t tableIndex;
    int32_t elementOffset;
    void **p;
    void *o;
    int32_t stack[21]; /* 84-byte helper output: data pointer at 0, callbacks at 0x2c/0x50. */

    /* Reference tests flag with beq-to-end (flag==0 block out-of-line), so the
     * nonzero path leads: `if (flag != 0) { ... }` fallthrough is flag==0. */
    if (flag != 0) {
        i = 0;
        tmp = D_80044B38.fn414(D_8005CCA8 + 0x32);
        OBJ_M114(objs[0], tmp, 4);
        OBJ_M114(objs[2], (int32_t)&D_8004935C[ctx->f5c * 0x66], -1);
        p = objs;
        do {
            elementOffset = i * 4 + ctx->f5c * 0x18;
            OBJ_M118(p[3], 0, *(int32_t *)(ctx->f50 + elementOffset + 0x74));
            o = p[3];
            OBJ_M148(o, 1);
            p += 1;
            i += 1;
        } while (i < 6);
        s0 = 0x1f;
        if (ctx->f440 < 0x3c)
            s0 = ctx->f43c + 0x1c;
        tmp = D_80044B38.fn414(D_8005CCA8 + 0x32);
        OBJ_M114(objs[1], tmp, s0);
        tableBase = (int32_t)D_80048D34;
        tableOffset = ctx->f60 + ctx->f64 * 9;
        tableIndex = ctx->f5c * 51;
        s0 = ((TableValue83d28 *)(tableBase + (tableIndex + tableOffset) * 2))->value;
        tmp = D_80044B38.fn414(D_8005CCA8 + 0x16);
        OBJ_M114(objs[9], tmp, s0);
        if (ctx->f43c == 0)
            goto lab_841b0;
        {
            F0x8001ebf8(stack);
            ((void (*)(int32_t))stack[11])(s0);
            ret = ((int32_t(*)(void))stack[20])();
            if (ret != 0) {
                OBJ_M144(objs[10], 0);
                OBJ_M144(objs[11], 0);
                tmp = D_80044B38.fn414(D_8005CCA8 + 0x1d);
                OBJ_M114(objs[16], tmp, s0);
                OBJ_M144(objs[12], 0);
                OBJ_M144(objs[13], 0);
                OBJ_M144(objs[14], 0);
                OBJ_M144(objs[15], 0);
                return;
            } else {
                tmp = D_80044B38.fn414(D_8005CCA8 + 0x32);
                OBJ_M114(objs[10], tmp, 8);
                OBJ_M118(objs[11], 0, ((uint8_t *)stack[0])[5]);
                OBJ_M148(objs[11], 1);
                if ((s0 == 0x45) || (s0 == 0x70) || (s0 == 0x9b) || (s0 == 0xc6) || (s0 == 0xf1)) {
                    tmp = D_80044B38.fn414(D_8005CCA8 + 0x1d);
                    OBJ_M114(objs[16], tmp, s0);
                    OBJ_M144(objs[12], 0);
                    OBJ_M144(objs[13], 0);
                    OBJ_M144(objs[14], 0);
                    OBJ_M144(objs[15], 0);
                    return;
                } else {
                    OBJ_M144(objs[16], 0);
                    tmp = D_80044B38.fn414(D_8005CCA8 + 0x32);
                    OBJ_M114(objs[12], tmp, 0x11);
                    OBJ_M118(objs[13], 0, ((uint8_t *)stack[0])[1]);
                    OBJ_M148(objs[13], 1);
                    tmp = D_80044B38.fn414(D_8005CCA8 + 0x32);
                    OBJ_M114(objs[14], tmp, 0x12);
                    OBJ_M118(objs[15], 0, ((uint8_t *)stack[0])[2]);
                    OBJ_M148(objs[15], 1);
                    return;
                }
            }
        }
    }
    OBJ_M144(objs[0], 0);
    i = 0;
    OBJ_M144(objs[2], 0);
    p = objs;
    do {
        OBJ_M144(p[3], 0);
        p += 1;
        i += 1;
    } while (i < 6);
    OBJ_M144(objs[1], 0);
    OBJ_M144(objs[9], 0);
lab_841b0:
    OBJ_M144(objs[10], 0);
    OBJ_M144(objs[11], 0);
    OBJ_M144(objs[16], 0);
    OBJ_M144(objs[12], 0);
    OBJ_M144(objs[13], 0);
    OBJ_M144(objs[14], 0);
    OBJ_M144(objs[15], 0);
}
