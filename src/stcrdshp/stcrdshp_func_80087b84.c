#include "common/types.h"

extern void STCRDSHP_func_80087a2c(void);
extern void *EXE_F0x80014504(void *, int32_t, int32_t);

void STCRDSHP_func_80087b84(void *callback, void *user_data) {
    uint8_t *object;

    object = EXE_F0x80014504(&STCRDSHP_func_80087a2c, 0xdc, 0x4c);
    *(int32_t *)(object + 0x54) = 0x1000;
    *(int32_t *)(object + 0x58) = 6;
    *(void **)(object + 0x50) = callback;
    *(void **)(object + 0xa0) = user_data;
}
