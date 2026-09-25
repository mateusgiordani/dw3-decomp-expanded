#include "common/types.h"

/* STCRDABM:0x8008314c (40 bytes, 10 instructions)
 * PAL-SLES-03936; reviewed body-8008314c, base 0x80082cb0.
 * The method pointer at object offset 0x2c is called with the object and 1.
 */
void STCRDABM_func_8008314c(void *object) {
    ((void (*)(void *, int))*(void **)((char *)object + 0x2c))(object, 1);
}
