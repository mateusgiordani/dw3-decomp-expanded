#include "common/types.h"

/* STCRDABM:0x80083114 (56 bytes, 14 instructions)
 * PAL-SLES-03936; reviewed body-80083114, base 0x80082cb0.
 */
void STCRDABM_func_80083114(void *object, int32_t value) {
    int32_t previous;

    previous = *(int32_t *)((char *)object + 0x5c);
    *(int32_t *)((char *)object + 0x5c) = value;
    *(int32_t *)((char *)object + 0x60) = previous;
    *(int32_t *)((char *)object + 0x68) = 0;
    *(int32_t *)((char *)object + 0x70) = 0;
    ((void (*)(void *, int32_t))*(void **)((char *)object + 0x28))(object, 2);
}
