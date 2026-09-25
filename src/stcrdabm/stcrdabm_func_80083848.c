#include "common/types.h"

/* STCRDABM:0x80083848 (560 bytes, 140 instructions)
 * PAL-SLES-03936; base 0x80082cb0, file offset 0xb98.
 * Reviewed portable C: album tick/state dispatcher. The word at object
 * offset 0x0c selects the path: 1 runs the 0x80083780 step then jumps to
 * the flag-0 draw arm; 2 runs the 0x10 sub-state machine (timer at
 * 0x18/0x6c via D0x8004df98, counters at 0x68/0x70, setup at 0x8008304c,
 * presence check at 0x80083734 with 0x4001c chime via D0x80055c48) then
 * the shared 0x800835bc refresh and the 0x80083174 draw with flag
 * (0x10 < 3 ? 1 : 0); 3 returns immediately; anything else runs the
 * method at object offset 0x38, stores 1 at 0x5c and calls 0x80083114
 * with 1. The draw call is written once per arm (state1 joins the
 * flag-0 arm via draw0); cross-jumping merges them into the single PAL
 * jal with the branchy beq/j/li/clear shape. The sub-3 tick reads go
 * through EXE global table D0x8004DE10 word 0x62 (0x8004DE10 + 0x188 ==
 * D0x8004df98); the other tick reads use D0x8004df98 directly, matching
 * the PAL s1-cached vs direct address modes. In sub-3 the second tick
 * is held in fresh across the 0x70 load so the branch-local cnt3 counter
 * stays in v1; the 0x6c store fills the load-delay slot and the slti
 * result lands in v1. Matching: psyq-gcc-2.8.1-sn32-4.0.0010 /
 * aspsx-2.79, variant base, -O2 -G0 (exact_byte_match 560/560).
 */
extern void STCRDABM_func_80083780(void *object);
extern void STCRDABM_func_8008304c(void *object);
extern void STCRDABM_func_800835bc(void *object);
extern void STCRDABM_func_80083114(void *object, int32_t value);
extern void STCRDABM_func_80083174(void *object, int32_t flag);
extern int32_t STCRDABM_func_80083734(void *object);
extern int32_t (*D0x8004df98)(void);
extern int32_t D0x8004DE10[];
extern void (*D0x80055c48)(int32_t);

void STCRDABM_func_80083848(void *object) {
    int32_t state;
    int32_t sub;
    int32_t tick;
    int32_t fresh;
    int32_t cnt3;

    state = *(int32_t *)((char *)object + 0x0c);
    if (state == 1)
        goto do_state1;
    if (state < 2)
        goto other;
    if (state == 2)
        goto do_state2;
    if (state == 3)
        return;
other:
    ((void (*)(void *))*(void **)((char *)object + 0x38))(object);
    *(int32_t *)((char *)object + 0x5c) = 1;
    STCRDABM_func_80083114(object, 1);
    return;
do_state1:
    STCRDABM_func_80083780(object);
    goto draw0;
do_state2:
    sub = *(int32_t *)((char *)object + 0x10);
    if (sub == 1)
        goto sub1;
    if (sub < 2)
        goto count;
    if (sub == 2)
        goto sub2;
    if (sub == 3)
        goto sub3;
count:
    tick = *(int32_t *)((char *)object + 0x68) + 1;
    *(int32_t *)((char *)object + 0x68) = tick;
    if (tick < 0x0c) {
        ((void (*)(void *))*(void **)((char *)object + 0x3c))(object);
        *(int32_t *)((char *)object + 0x18) = D0x8004df98();
    } else {
        *(int32_t *)((char *)object + 0x68) = 0x0c;
        *(int32_t *)((char *)object + 0x10) = 2;
    }
    goto refresh;
sub1:
    tick = D0x8004df98();
    if (tick - *(int32_t *)((char *)object + 0x18) > 1)
        *(int32_t *)((char *)object + 0x10) = *(int32_t *)((char *)object + 0x14);
    goto refresh;
sub2:
    STCRDABM_func_8008304c(object);
    *(int32_t *)((char *)object + 0x64) = 0x0c;
    *(int32_t *)((char *)object + 0x6c) = D0x8004df98();
    ((void (*)(void *))*(void **)((char *)object + 0x3c))(object);
    if (STCRDABM_func_80083734(object) != 0)
        D0x80055c48(0x4001c);
    goto refresh;
sub3:
    tick = ((int32_t (*)(void))D0x8004DE10[0x62])();
    if (tick - *(int32_t *)((char *)object + 0x6c) > 1) {
        fresh = ((int32_t (*)(void))D0x8004DE10[0x62])();
        cnt3 = *(int32_t *)((char *)object + 0x70) + 1;
        *(int32_t *)((char *)object + 0x70) = cnt3;
        *(int32_t *)((char *)object + 0x6c) = fresh;
        if (cnt3 > 10)
            *(int32_t *)((char *)object + 0x0c) = 1;
    }
refresh:
    STCRDABM_func_800835bc(object);
    if (*(int32_t *)((char *)object + 0x10) < 3)
        STCRDABM_func_80083174(object, 1);
    else
draw0:
        STCRDABM_func_80083174(object, 0);
}
