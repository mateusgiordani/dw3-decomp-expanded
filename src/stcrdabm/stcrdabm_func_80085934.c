#include "common/types.h"

/* STCRDABM:0x80085934 (108 bytes, 27 instructions)
 * PAL-SLES-03936; reviewed body-80085934, base 0x80082cb0.
 */
int32_t STCRDABM_func_80085934(int32_t object) {
    int32_t value;

    if (*(int32_t *)(object + 0xc) == 0) {
        return 1;
    }
    value = *(int32_t *)(object + 8) + *(int32_t *)(object + 4);
    *(int32_t *)(object + 8) = value;
    if (*(int32_t *)(object + 4) > 0) {
        if (value > 0x1000) {
            *(int32_t *)(object + 8) = 0x1000;
            *(int32_t *)(object + 0xc) = 0;
            return 1;
        }
    } else if (value < 0) {
        *(int32_t *)(object + 8) = 0;
        *(int32_t *)(object + 0xc) = 0;
        return 1;
    }
    return 0;
}
