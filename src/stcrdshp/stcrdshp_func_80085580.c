/* STCRDSHP:0x80085580, 180-byte PAL body, base 0x80082cb0.
 * Same state-dispatch + accumulate/clamp shape as STCRDDEK:0x800834e0
 * (proven exact_byte_match): int32_t-indexed object, forward gotos,
 * separate "next" local so li v0,2 duplicates into both branch delay
 * slots with one shared sw at the join. No inline ASM. */
#include "common/types.h"

extern void STCRDSHP_func_8008543c(int32_t *arg0);

void STCRDSHP_func_80085580(int32_t *arg0) {
    int32_t state;
    int32_t sum;
    int32_t next;

    state = arg0[3];
    if (state == 1) {
        goto state1;
    }
    if (state < 2) {
        goto indirect;
    }
    if (state == 2) {
        goto tail;
    }
    if (state == 3) {
        return;
    }
indirect:
    ((void (*)(int32_t *))arg0[14])(arg0);
    return;
state1:
    if (arg0[4] == 0) {
        return;
    }
    sum = arg0[23] + arg0[24];
    arg0[23] = sum;
    if (arg0[22] != 0) {
        goto negdir;
    }
    if (sum <= 0xFF00) {
        goto tail;
    }
    next = 2;
    arg0[23] = 0xFF00;
    goto set2;
negdir:
    if (sum >= 0) {
        goto tail;
    }
    next = 2;
    arg0[23] = 0;
set2:
    arg0[3] = next;
tail:
    STCRDSHP_func_8008543c(arg0);
}
