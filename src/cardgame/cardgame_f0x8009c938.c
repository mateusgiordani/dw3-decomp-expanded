#include "common/types.h"

extern void CARDGAME_F0x8009c898(int32_t a0, int32_t a1, int32_t a2, int32_t a3, int32_t sp10, int32_t sp14);

void CARDGAME_F0x8009c938(int32_t a0, int32_t a1, int32_t a2, int32_t a3, int32_t arg4)
{
    CARDGAME_F0x8009c898(a0, a1, a2, a3, arg4, 0);
}
