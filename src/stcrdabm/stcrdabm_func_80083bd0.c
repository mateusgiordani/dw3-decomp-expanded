#include "common/types.h"

/* STCRDABM:0x80083bd0 (44 bytes, 11 instructions)
 * PAL-SLES-03936; reviewed body-80083bd0, base 0x80082cb0.
 * Installs the STCRDABM callback at 0x80083ad8 with task parameters 0x50, 4.
 */
extern int32_t F0x80014504(int32_t callback, int32_t size, int32_t mode);
extern void STCRDABM_func_80083ad8(void);

void STCRDABM_func_80083bd0(void) {
    F0x80014504((int32_t)STCRDABM_func_80083ad8, 0x50, 4);
}
