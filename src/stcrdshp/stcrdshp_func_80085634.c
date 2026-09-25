#include "common/types.h"

extern void STCRDSHP_func_80085580(void);
extern void STCRDSHP_func_800853b4(void);
extern void *EXE_F0x80014504(void *, int32_t, int32_t);

void STCRDSHP_func_80085634(void) {
    uint8_t *object;

    object = EXE_F0x80014504(&STCRDSHP_func_80085580, 0x68, 0);
    *(void (**)(void))(object + 0x64) = STCRDSHP_func_800853b4;
    *(int32_t *)(object + 0x50) = 0x1000;
    *(int32_t *)(object + 0x54) = 0;
}
