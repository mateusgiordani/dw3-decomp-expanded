#include "common/types.h"

extern void STCRDSHP_func_80087bdc(void);
extern void EXE_F0x80014504(void *, int32_t, int32_t);

void STCRDSHP_func_80087cd4(void) {
    EXE_F0x80014504(&STCRDSHP_func_80087bdc, 0x50, 4);
}
