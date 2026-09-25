#include "common/types.h"

extern void STCRDSHP_func_80085cd8(void *);
extern void STCRDSHP_func_800857b0(void *, int32_t);
extern void STCRDSHP_func_80085bac(void *);
extern void STCRDSHP_func_80085678(void *);
extern void STCRDSHP_func_80085730(void *, void *);

/* STCRDSHP:0x80085da0 (572 bytes): card-shop task state dispatcher.
 * PAL dispatch order is mode ==1, <2, ==2, ==3 with the ==1 body out of
 * line; step dispatch mirrors it (==1, <2, ==2, ==3, shared default).
 * Single cursor keeps self in s0 (0x20 frame). The if-goto/label funnels
 * keep each multi-entry check a single branch, as in STCRDDEK_func_800831f0.
 * Timer calls go through D_8004DE10[0x62] (0x8004df98); the two step-3
 * calls share the 0x8004de10 base in s1 with 0x188 offsets. */
extern int32_t D_8004DE10[];
#define WORD(off) (*(int32_t *)(self + (off)))
#define CLOCK() (((int32_t (*)(void))D_8004DE10[0x62])())
#define EVENT(v) ((*(void (**)(uint32_t))0x80055c48)(v))
void STCRDSHP_func_80085da0(uint8_t *self)
{
    int32_t step, value;
    int32_t mode = WORD(0x0c);
    if (mode == 1)
        goto amode;
    if (mode < 2) {
        (*(void (**)(void *))(self + 0x38))(self);
        STCRDSHP_func_80085730(self, self + 0x7c);
    } else if (mode != 2) {
        if (mode == 3) {
        } else {
            (*(void (**)(void *))(self + 0x38))(self);
            STCRDSHP_func_80085730(self, self + 0x7c);
            return;
        amode:
            STCRDSHP_func_80085cd8(self);
            STCRDSHP_func_800857b0(self, 0);
        }
    } else {
        step = WORD(0x10);
        if (step == 1)
            goto smode;
        if (step < 2)
            goto def_step;
        if (step == 2)
            goto s2mode;
        if (step == 3)
            goto s3mode;
        def_step:
            value = WORD(0x68) + 1;
            WORD(0x68) = value;
            if (value < 6) {
                (*(void (**)(void *))(self + 0x3c))(self);
                WORD(0x18) = CLOCK();
            } else {
                WORD(0x68) = 6;
                WORD(0x10) = 2;
            }
            EVENT(0x800460bd);
            goto step_join;
        smode:
            value = CLOCK();
            if (value - WORD(0x18) > 1) WORD(0x10) = WORD(0x14);
            goto step_join;
        s2mode:
            STCRDSHP_func_80085678(self);
            WORD(0x64) = 6;
            WORD(0x6c) = CLOCK();
            (*(void (**)(void *))(self + 0x3c))(self);
            EVENT(0x4001c);
            goto step_join;
        s3mode:
            value = CLOCK();
            if (value - WORD(0x6c) > 1) {
                WORD(0x6c) = CLOCK();
                if (++WORD(0x70) > 10) WORD(0x0c) = 1;
            }
        step_join:
        STCRDSHP_func_80085bac(self);
        if (WORD(0x10) < 3) {
            STCRDSHP_func_800857b0(self, 1);
        } else {
            STCRDSHP_func_800857b0(self, 0);
        }
    }
}
