/* CARDGAME:0x8009a5b0, 124 PAL bytes; 0x8009a5b8 is the internal framed entry.
 * The direct caller at 0x8009ba10 targets the global callback-load prefix. */
#include "common/types.h"

typedef int32_t (*CardTick9a5b0)(void);
extern CardTick9a5b0 D_8004df9c;
int32_t CARDGAME_F0x8009a5b0(void *unused, uint8_t *ctx, int32_t limit)
{
    int32_t result = 0;
    int32_t value;
    int32_t elapsed;
    (void)unused;
    elapsed = D_8004df9c();
    value = *(int32_t *)(ctx + 0x28);
    value += elapsed;
    *(int32_t *)(ctx + 0x28) = value;
    if (value >= limit) {
        ctx[0x42] = 1;
        result = 1;
        *(int32_t *)(ctx + 0x2c) = 0;
        *(int32_t *)(ctx + 0x28) = 0;
        *(int16_t *)(ctx + 0x26) = 0;
        ctx[0x47] = 0;
    }
    return result;
}
