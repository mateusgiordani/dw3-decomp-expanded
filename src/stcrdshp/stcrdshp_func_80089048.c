#include "common/types.h"

int32_t STCRDSHP_func_80089048(int32_t *state) {
    int32_t value;

    if (state[3] == 0) {
        return 1;
    }
    value = state[2] + state[1];
    state[2] = value;
    if (state[1] > 0) {
        if (value > 0x1000) {
            state[2] = 0x1000;
            state[3] = 0;
            return 1;
        }
    } else if (value < 0) {
        state[2] = 0;
        state[3] = 0;
        return 1;
    }
    return 0;
}
