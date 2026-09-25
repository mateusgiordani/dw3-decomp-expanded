#include "common/types.h"

typedef struct {
    int32_t steps;   /* 0x00 */
    int32_t unk04;   /* 0x04 */
    int32_t start;   /* 0x08 */
    int32_t fixed;   /* 0x0c */
    int32_t end;     /* 0x10 */
    int32_t step;    /* 0x14 */
    int32_t active;  /* 0x18 */
} STCRDSHP_Interp;

void STCRDSHP_func_800890b4(STCRDSHP_Interp *state, int32_t start, int32_t end,
                            int32_t steps) {
    if (start != end) {
        state->steps = steps;
        state->fixed = start << 8;
        state->start = start;
        state->end = end;
        state->active = 1;
        state->step = ((end - start) << 8) / state->steps;
    }
}
