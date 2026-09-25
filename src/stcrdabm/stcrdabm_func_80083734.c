#include "common/types.h"

extern uint8_t D0x80048d34[];

/* STCRDABM:0x80083734 (76 bytes, 19 instructions)
 * PAL-SLES-03936; reviewed body-80083734, base 0x80082cb0.
 */
int32_t STCRDABM_func_80083734(int32_t object) {
    int32_t index;

    index = 0;
    object = *(int32_t *)(object + 0x5c);
    do {
        if (D0x80048d34[object + index + 0x4df] != 0 ||
            object + index >= 0x13b) {
            return 1;
        }
        index++;
    } while (index < 0xc);
    return 0;
}
