#include "common/types.h"

extern void CARDGAME_F0x8008baa4(uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3);

void CARDGAME_F0x8008bc6c(uint32_t a0, uint32_t a1, uint32_t a2)
{
    CARDGAME_F0x8008baa4(a0, a1, a2, 0);
}
