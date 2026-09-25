// STCRDSHP:0x80085fdc, 152-byte PAL body. Initialize the loop counter before
// the cursor and increment it before loading each copied word, as in PAL.
#include "common/types.h"

extern void STCRDSHP_func_80085da0(void);
extern void STCRDSHP_func_80085730(void);
extern void STCRDSHP_func_80085788(void);
extern void *EXE_F0x80014504(void *, int32_t, int32_t);

void *STCRDSHP_func_80085fdc(void *callback, uint32_t *values) {
    uint8_t *object;
    uint8_t *entry;
    int32_t i;

    object = EXE_F0x80014504(&STCRDSHP_func_80085da0, 0xc4, 0);
    i = 0;
    entry = object;
    *(void (**)(void))(object + 0xbc) = STCRDSHP_func_80085730;
    *(void (**)(void))(object + 0xc0) = STCRDSHP_func_80085788;
    *(int32_t *)(object + 0x54) = 0x1000;
    *(int32_t *)(object + 0x58) = 6;
    *(void **)(object + 0x50) = callback;
    do {
        i++;
        *(uint32_t *)(entry + 0x7c) = *values++;
        entry += 4;
    } while (i < 6);
    return object;
}
