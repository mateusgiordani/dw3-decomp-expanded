#include "common/types.h"

/* STCRDABM:0x80083008 (68 bytes, 17 instructions)
 * PAL-SLES-03936; reviewed body-80083008, base 0x80082cb0.
 */
extern int32_t F0x80014504(int32_t callback, int32_t size, int32_t mode);
extern void STCRDABM_func_80082f54(void);
extern void STCRDABM_func_80082d88(void);

void STCRDABM_func_80083008(void) {
    int32_t object;

    object = F0x80014504((int32_t)STCRDABM_func_80082f54, 0x68, 0);
    *(int32_t *)(object + 0x64) = (int32_t)STCRDABM_func_80082d88;
    *(int32_t *)(object + 0x50) = 0x1000;
    *(int32_t *)(object + 0x54) = 0;
}
