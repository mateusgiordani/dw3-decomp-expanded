#include "common/types.h"

extern void STCRDSHP_func_800852cc(void);
extern void *EXE_F0x80014504(void *, int32_t, int32_t);

void STCRDSHP_func_8008536c(void *callback) {
    uint8_t *object;

    object = EXE_F0x80014504(&STCRDSHP_func_800852cc, 0x730, 0x78);
    *(int32_t *)(object + 0x54) = 0x1000;
    *(int32_t *)(object + 0x58) = 6;
    *(void **)(object + 0x50) = callback;
}
