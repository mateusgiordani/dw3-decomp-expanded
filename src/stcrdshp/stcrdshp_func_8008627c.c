#include "common/types.h"

/* STCRDSHP:0x8008627c (1232 bytes: 0x8008627c..0x8008674c; body ends jr ra 0x80086744)
 * PAL-SLES-03936; reviewed body-8008627c, base 0x80082cb0 (file off 13772).
 * Card-shop setup/dispatch: derives a 16-bit card id from src, then either
 * disables eight objects (flag == 0) or programs object method slots via the
 * EXE helper vector (base D_80044B38 + 0x414) and a stack setup block filled
 * by EXE_F0x8001ebf8 (0x54-byte setup, verified against PAL). Callers at
 * 0x80086e68, 0x8008706c and 0x8008799c discard the result. The store in the
 * first caller's jal delay slot executes before this function, not after it.
 * NOTE (uncertain): the +0x1d helper call on the fn_check()==0 card-id-match
 * path loads *(0x80044F4C) via an absolute lui/lw instead of base + 0x414,
 * so it is written as a direct data-pointer call (D_80044F4C); all other
 * helper calls use base + 0x414. Same slot, different compiler synthesis.
 */

typedef int32_t (*STCRDSHP_HelperFn)(int32_t);

typedef struct {
    uint8_t unk_00[0x5C]; /* +0x00 */
    int32_t idx_5C;       /* +0x5C */
    uint8_t unk_60[0x0C]; /* +0x60 */
    int32_t idx_6C;       /* +0x6C */
    uint8_t unk_70[0x38]; /* +0x70..0xA7 */
    void *ptr_A8;         /* +0xA8 */
} STCRDSHP_8627c_Src; /* layout uncertain beyond used offsets */

typedef struct {
    uint8_t *blob;               /* +0x00: bytes +1/+2/+5 read */
    uint8_t unk_04[0x28];        /* +0x04..0x2B */
    int32_t (*fn_card)(int32_t); /* +0x2C */
    uint8_t unk_30[0x20];        /* +0x30..0x4F */
    int32_t (*fn_check)(void);   /* +0x50 */
} STCRDSHP_8627c_Setup; /* layout uncertain; filled by EXE_F0x8001ebf8 */

extern uint8_t D_80044B38[];
extern int32_t D_8005CCA8;
extern int8_t D_80048D34[];
extern void *D_8008CB9C; /* RAM-resident function pointer */
extern STCRDSHP_HelperFn D_80044F4C;
extern void EXE_F0x8001ebf8(void *);

#define STCRDSHP_HELP(base, id) ((*(STCRDSHP_HelperFn *)((base) + 0x414))((id)))
#define STCRDSHP_M3(arr, i, off, a, b) ((*(int32_t (**)(void *, int32_t, int32_t))((uint8_t *)(arr)[i] + (off)))((arr)[i], (a), (b)))
#define STCRDSHP_M2(arr, i, off, a) ((*(int32_t (**)(void *, int32_t))((uint8_t *)(arr)[i] + (off)))((arr)[i], (a)))

void STCRDSHP_func_8008627c(STCRDSHP_8627c_Src *src, void **objs, int32_t flag) {
    uint8_t *base;
    STCRDSHP_8627c_Setup setup;
    int32_t cid;
    int32_t r;
    int8_t *btab;

    cid = *(int16_t *)((src->idx_6C * 6 + src->idx_5C) * 2 + *(int32_t *)((uint8_t *)src->ptr_A8 + 8));
    /* PAL activation falls through; the hide path is placed at the end. */
    if (flag != 0) {
        EXE_F0x8001ebf8(&setup);
        setup.fn_card(cid);
        base = D_80044B38;
        r = STCRDSHP_HELP(base, D_8005CCA8 + 0x16);
        STCRDSHP_M3(objs, 0, 0x114, r, cid);
        r = STCRDSHP_HELP(base, D_8005CCA8 + 0x32);
        STCRDSHP_M3(objs, 10, 0x114, r, 3);
        r = (int16_t)((int32_t (*)(int32_t))D_8008CB9C)(cid);
        STCRDSHP_M3(objs, 11, 0x118, 0, r);
        STCRDSHP_M2(objs, 11, 0x148, 1);
        r = STCRDSHP_HELP(base, D_8005CCA8 + 0x32);
        STCRDSHP_M3(objs, 3, 0x114, r, 8);
        btab = D_80048D34 + cid;
        STCRDSHP_M3(objs, 4, 0x118, 0, btab[0x3A2]);
        STCRDSHP_M2(objs, 4, 0x148, 1);
        if (setup.fn_check() != 0) {
            STCRDSHP_M2(objs, 1, 0x144, 0);
            STCRDSHP_M2(objs, 2, 0x144, 0);
            r = STCRDSHP_HELP(base, D_8005CCA8 + 0x1D);
            STCRDSHP_M3(objs, 5, 0x114, r, cid);
            /* Keep each real exit complete; late cross-jump shares only this tail. */
            STCRDSHP_M2(objs, 6, 0x144, 0);
            STCRDSHP_M2(objs, 7, 0x144, 0);
            STCRDSHP_M2(objs, 8, 0x144, 0);
            STCRDSHP_M2(objs, 9, 0x144, 0);
            return;
        } else {
            r = STCRDSHP_HELP(base, D_8005CCA8 + 0x32);
            STCRDSHP_M3(objs, 1, 0x114, r, 8);
            STCRDSHP_M3(objs, 2, 0x118, 0, setup.blob[5]);
            STCRDSHP_M2(objs, 2, 0x148, 1);
            if (cid == 0x45 || cid == 0x70 || cid == 0x9B || cid == 0xC6 || cid == 0xF1) {
                r = D_80044F4C(D_8005CCA8 + 0x1D);
                STCRDSHP_M3(objs, 5, 0x114, r, cid);
                STCRDSHP_M2(objs, 6, 0x144, 0);
                STCRDSHP_M2(objs, 7, 0x144, 0);
                STCRDSHP_M2(objs, 8, 0x144, 0);
                STCRDSHP_M2(objs, 9, 0x144, 0);
                return;
            } else {
                STCRDSHP_M2(objs, 5, 0x144, 0);
                r = STCRDSHP_HELP(base, D_8005CCA8 + 0x32);
                STCRDSHP_M3(objs, 6, 0x114, r, 0x11);
                STCRDSHP_M3(objs, 7, 0x118, 0, setup.blob[1]);
                STCRDSHP_M2(objs, 7, 0x148, 1);
                r = STCRDSHP_HELP(base, D_8005CCA8 + 0x32);
                STCRDSHP_M3(objs, 8, 0x114, r, 0x12);
                STCRDSHP_M3(objs, 9, 0x118, 0, setup.blob[2]);
                STCRDSHP_M2(objs, 9, 0x148, 1);
                return;
            }
        }
    } else {
        STCRDSHP_M2(objs, 0, 0x144, 0);
        STCRDSHP_M2(objs, 10, 0x144, 0);
        STCRDSHP_M2(objs, 11, 0x144, 0);
        STCRDSHP_M2(objs, 3, 0x144, 0);
        STCRDSHP_M2(objs, 4, 0x144, 0);
        STCRDSHP_M2(objs, 1, 0x144, 0);
        STCRDSHP_M2(objs, 2, 0x144, 0);
        STCRDSHP_M2(objs, 5, 0x144, 0);
    }
    STCRDSHP_M2(objs, 6, 0x144, 0);
    STCRDSHP_M2(objs, 7, 0x144, 0);
    STCRDSHP_M2(objs, 8, 0x144, 0);
    STCRDSHP_M2(objs, 9, 0x144, 0);
    return;
}
