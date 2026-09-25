/* CARDGAME:0x80084090, 64 PAL bytes. Direct PAL callers target the
 * global callback load eight bytes before the former framed entry.
 * The callback is loaded from 0x8004df9c; no incoming-v0 ABI.
 * Evidence: reports/handoffs/cardgame-80084098-c-recovery.md. */
#include "common/types.h"

typedef int32_t (*CardCallback84090)(void);
extern CardCallback84090 D_8004df9c;

int32_t CARDGAME_F0x80084090(void *ctx)
{
    int32_t value = D_8004df9c();
    int32_t field = *(int32_t *)((uint8_t *)ctx + 0x424);
    field -= value;
    *(int32_t *)((uint8_t *)ctx + 0x424) = field;
    return field < 1;
}
