/* CARDGAME:0x8009a540, 112 PAL bytes. Direct PAL callers target the
 * global callback load eight bytes before the former framed entry.
 * The callback is loaded from 0x8004df9c; no incoming-v0 ABI.
 * Evidence: reports/handoffs/cardgame-8009a548-c-recovery.md. */
#include "common/types.h"

typedef int32_t (*CardTick9a540)(void);
extern CardTick9a540 D_8004df9c;
int32_t CARDGAME_F0x8009a540(void *unused, uint8_t *ctx)
{
    int32_t value;
    int32_t elapsed;
    int32_t result = 0;
    (void)unused;
    elapsed = D_8004df9c();
    value = *(int32_t *)(ctx + 0x28);
    value += elapsed;
    *(int32_t *)(ctx + 0x28) = value;
    if (value >= 10) {
        result = 1;
        ctx[0x42] = 11;
        *(int32_t *)(ctx + 0x2c) = 0;
        *(int32_t *)(ctx + 0x28) = 0;
        *(int16_t *)(ctx + 0x26) = 0;
        ctx[0x47] = 0;
    }
    return result;
}
