#include "common/types.h"

extern uint32_t D_80042B98[];

void STCRDSHP_func_800843c0(void *object) {
    int32_t count;
    int32_t index;
    int32_t match_count;
    int32_t total;

    match_count = ((int32_t (*)(int32_t, void *))D_80042B98[3])(1, (char *)object + 0x3c4);
    index = 0;
    *(int32_t *)((char *)object + 0x3c0) = 0;
    if (match_count > 0) {
        do {
            count = ((int32_t (*)(int32_t))D_80042B98[1])
                (*(int16_t *)((char *)object + index * 2 + 0x3c4));
            if (count == 0x62) {
                *(int16_t *)((char *)object + ((*(int32_t *)((char *)object + 0x3c0))++ << 1) + 0x98) =
                    *(uint16_t *)((char *)object + index * 2 + 0x3c4);
            }
            index++;
        } while (index < match_count);
    }
    total = *(int32_t *)((char *)object + 0x3c0);
    if (total != 0) {
        *(int32_t *)((char *)object + 0x64) = total / 8 + ((total & 7) != 0);
    }
}
