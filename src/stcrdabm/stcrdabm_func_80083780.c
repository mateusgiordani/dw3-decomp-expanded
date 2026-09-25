#include "common/types.h"

extern int32_t (*D0x8004df98)(void);

/* STCRDABM:0x80083780 (200 bytes), PAL-SLES-03936. */
void STCRDABM_func_80083780(void *object) {
    int32_t state;
    int32_t value;

    state = *(int32_t *)((char *)object + 0x10);
    if (state == 1)
        goto do_state1;
    if (state < 2)
        goto done;
    if (state == 2)
        goto do_state2;
    goto done;
do_state1:
    value = *(int32_t *)((char *)object + 0x64);
    if (value == 0)
        goto zero;
    *(int32_t *)((char *)object + 0x64) = value - 1;
    ((void (*)(void *))*(void **)((char *)object + 0x3c))(object);
    *(int32_t *)((char *)object + 0x18) = D0x8004df98();
    goto done;
zero:
    *(int32_t *)((char *)object + 0x0c) = 3;
    goto done;
do_state2:
    value = D0x8004df98();
    if (value - *(int32_t *)((char *)object + 0x18) > 1)
        *(int32_t *)((char *)object + 0x10) = 1;
done:
    ;
}
