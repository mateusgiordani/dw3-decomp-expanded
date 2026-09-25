#include "common/types.h"

/* STCRDABM:0x8008566c (60 bytes, 15 instructions)
 * PAL-SLES-03936; reviewed body-8008566c, base 0x80082cb0.
 */
extern int32_t F0x80014504(int32_t callback, int32_t size, int32_t mode);
extern void STCRDABM_func_80085540(void);

void STCRDABM_func_8008566c(void) {
    int32_t object;

    object = F0x80014504((int32_t)STCRDABM_func_80085540, 0xd8, 0x4c);
    *(int32_t *)(object + 0x50) = 0x1000;
    *(int32_t *)(object + 0x54) = 7;
}
