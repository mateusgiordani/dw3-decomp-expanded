#include "common/types.h"

extern int32_t F0x80014504(int32_t callback, int32_t size, int32_t mode);
extern void STCRDABM_func_80083848(void);
extern void STCRDABM_func_80083114(void);
extern void STCRDABM_func_8008314c(void);

/* STCRDABM:0x80083a78 (96 bytes, 24 instructions); PAL reviewed body. */
void STCRDABM_func_80083a78(int32_t value) {
    int32_t object;

    object = F0x80014504((int32_t)STCRDABM_func_80083848, 0x8c, 0);
    *(int32_t *)(object + 0x84) = (int32_t)STCRDABM_func_80083114;
    *(int32_t *)(object + 0x88) = (int32_t)STCRDABM_func_8008314c;
    *(int32_t *)(object + 0x54) = 0x1000;
    *(int32_t *)(object + 0x58) = 6;
    *(int32_t *)(object + 0x50) = value;
}
