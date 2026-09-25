#include "common/types.h"

extern void STCRDDEK_func_800831f0(void);
extern void EXE_F0x80014504(void *, int32_t, int32_t);

void STCRDDEK_func_800832e8(void) {
    EXE_F0x80014504(&STCRDDEK_func_800831f0, 0x50, 4);
}
